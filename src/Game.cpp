//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Game.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>

#include "Obstacle.h"

//using irrklang::ISoundEngine;
using irrklang::vec3df;
using irrklang::ISound;

Game::~Game() {
    // It seems it is necessary for the obstacles' destructors to be called first
    // Maybe because the sound engine could get destroyed sooner, causing an error
    _obstacles.clear();

    _safety_sound->drop();

    if (_sound_engine) {
        _sound_engine->drop();
    }
}

Game::Game(irrklang::ISoundEngine *sound_engine)
    : _sound_engine(sound_engine),
      _time_since_action_start(0), _action_in_progress(false),
      _level(1), _row(0), _game_over(false) {
//    _rng = std::mt19937(SEED);
    _rng = std::mt19937(time(0));

    loadSounds();
//    _sound_engine->setDopplerEffectParameters(10, 1.0);
    _sound_engine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));

    _safety_sound = _sound_engine->play2D(
        (SOUND_DIR + SAFETY_SOUND_FILE).c_str(),
        true, true
    );
    _safety_sound->setVolume(0);
    _safety_sound->setIsPaused(false);

    startNewLevel();
}


void Game::startNewLevel() {
    assert(_obstacles.empty());
    std::cerr << "Starting new level" << std::endl;
    _row = 0;

    // Start with a "free" obstacle (no danger)
    _obstacles.push_back(std::make_unique<Obstacle>(0));

    auto playSound = [&](const std::string &filename) {
        ISound *sound = _sound_engine->play3D(
            (SOUND_DIR + filename).c_str(),
            vec3df(0, 0, 0),
            true, false, true
        );
        if (!sound) {
            throw std::runtime_error("Couldn't load sound: " +
                                     std::string(filename));
        }
        return sound;
    };

    for (int i = 0; i < N_OBSTACLES; ++i) {
        if (_level < MIN_LEVEL_FOR_OBSTACLE[i]) {
            break;
        }

        ISound *main_sound = playSound(OBSTACLE_SOUND_FILES[i]);
        ISound *warning_sound = playSound(WARNING_SOUND_FILES[i]);

        _obstacles.push_back(std::make_unique<Obstacle>(main_sound, warning_sound, 0, 0, i + 1));
        _obstacles.back()->randomizePosition(_rng, _level);
    }
}

void Game::step(float dt, bool action) {
    if (_game_over) {
        if (action) {
            _level = 1;
            _game_over = false;
            startNewLevel();
        }
        return;
    }
    if (action && !_action_in_progress) {
        // Begin action
        _time_since_action_start = 0;
        _action_in_progress = true;
        _row++;

        // Play sound effect
        int action_sound_index = (_row > 1 && _obstacles.size() == 1) ? 0 : _row;
        ISound *action_sound = _sound_engine->play2D(
            (SOUND_DIR + ACTION_SOUND_FILES[action_sound_index]).c_str(), false, true
        );
        action_sound->setVolume(ACTION_VOLUME);
        action_sound->setIsPaused(false);
    }

    _time_since_action_start += dt;
    float action_progress = 0;

    if (_action_in_progress) {
        action_progress = _time_since_action_start / ACTION_DURATION;
        if (action_progress >= 1) {
            finishAction();
            if (_obstacles.empty()) {
                _level++;
                startNewLevel();
            }
        } else {
            if (_obstacles.size()) {
                _obstacles[0]->setFadeout(action_progress);
            }
        }
    }

    updateSafetyVolume();

    for (size_t i = 0; i < _obstacles.size(); ++i) {
        _obstacles[i]->step(dt, i);
    }

    // Lose when there is a collision with the first obstacle, or also with the second if we're
    // in a transition
    int collision = -1;
    if (std::abs(_obstacles[0]->getX()) < COLLISION_DISTANCE) {
        collision = 0;
    } else if (_action_in_progress
               && _obstacles.size() > 1
               && std::abs(_obstacles[1]->getX()) < COLLISION_DISTANCE) {
        collision = 1;
    }
    if (collision >= 0) {
        ISound *loss_sound = _sound_engine->play2D(
            (SOUND_DIR + LOSS_SOUND_FILES[_obstacles[collision]->getIndex() - 1]).c_str(),
            false, true
        );
        loss_sound->setVolume(LOSS_VOLUME);
        loss_sound->setIsPaused(false);

        lose();
    }
}

void Game::loadSounds() {
    auto loadSoundSource = [&](const std::string &name) {
        auto *sound_source = _sound_engine->addSoundSourceFromFile(
            (SOUND_DIR + name).c_str(),
            irrklang::ESM_AUTO_DETECT,
            true // preload the sound
        );
        if (!sound_source) {
            throw std::runtime_error("Couldn't load sound source: " + name);
        }
        return sound_source;
    };
    // Obstacles
    std::set<std::string> files_to_load;
    files_to_load.insert(SAFETY_SOUND_FILE);

    for (const std::string &name : OBSTACLE_SOUND_FILES) {
        files_to_load.insert(name);
    }

    for (const std::string &name : WARNING_SOUND_FILES) {
        files_to_load.insert(name);
    }

    for (const std::string &name : LOSS_SOUND_FILES) {
        files_to_load.insert(name);
    }

    for (const std::string &name : ACTION_SOUND_FILES) {
        files_to_load.insert(name);
    }

    for (const std::string &name : files_to_load) {
        auto *sound_source = loadSoundSource(name);
        _sound_sources.push_back(sound_source);
    }
}

void Game::finishAction() {
    _action_in_progress = false;
    std::cout << "action finished" << std::endl;
    assert(!_obstacles.empty());
    _obstacles.erase(_obstacles.begin());
}

void Game::lose() {
    _game_over = true;
    _obstacles.clear();
    _action_in_progress = false;
    _time_since_action_start = 0;
    _safety_sound->setVolume(0);

    std::cout << "You lost! You reached level " << _level << "." << std::endl;
    std::cout << "Press Q to quit, press Enter to play again." << std::endl;
}

void Game::updateSafetyVolume() {
    assert(!_obstacles.empty());
    float volume = 0;
    if (!_action_in_progress) {
        volume = _obstacles[0]->isFree() * SAFETY_VOLUME;
    } else {
        float action_progress = _time_since_action_start / ACTION_DURATION;
        if (_obstacles[0]->isFree()) {
            volume = SAFETY_VOLUME * (1 - action_progress);
        } else if (_obstacles.size() == 1 || _obstacles[1]->isFree()) {
            volume = SAFETY_VOLUME * action_progress;
        }
    }
    _safety_sound->setVolume(volume);

}
