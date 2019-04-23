//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Game.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <set>
#include <cassert>

#include "Obstacle.h"

using irrklang::vec3df;
using irrklang::ISound;

Game::~Game() {
    // It seems it is necessary for the obstacles' destructors to be called first
    // Maybe because the sound engine could get destroyed sooner, causing an error
    _obstacles.clear();

    // IrrKlang requires us to call drop() on all ISounds
    _safety_sound->drop();

    if (_sound_engine) {
        _sound_engine->drop();
    }
}

Game::Game(irrklang::ISoundEngine *sound_engine, bool tutorial)
    : _sound_engine(sound_engine),
      _time_since_action_start(0), _action_in_progress(false),
      _level(tutorial ? 0 : 1), _row(0), _game_over(false), _tutorial(tutorial) {
    _rng = std::mt19937(time(0));

    loadSounds();
//    The Doppler effect is not very noticeable
//    _sound_engine->setDopplerEffectParameters(1., 10.0);
    _sound_engine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));

    _safety_sound = _sound_engine->play2D(
        (SOUND_DIR + SAFETY_SOUND_FILE).c_str(),
        true, true
    );
    _safety_sound->setVolume(0);
    _safety_sound->setIsPaused(false);

    startNewLevel();
    showTutorialMessage();
}


void Game::startNewLevel() {
    assert(_obstacles.empty());
    std::cout << ROW_SYMBOLS[0] << " " << _level << std::endl;
    _row = 0;

    // Start with a "free" obstacle (no danger)
    _obstacles.push_back(std::make_unique<Obstacle>(0));

    auto playSound = [&](const std::string &filename) {
        ISound *sound = _sound_engine->play3D(
            (SOUND_DIR + filename).c_str(),
            vec3df(0, 0, 0),
            true, true, true
        );
        if (!sound) {
            throw std::runtime_error("Couldn't load sound: " +
                                     std::string(filename));
        }
        // Without setting the volume here, the sound might play at volume 1 for a brief period
        sound->setVolume(0);
        sound->setIsPaused(false);
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
        _obstacles.back()->setVolume(0);
    }
}

void Game::step(float dt, bool action) {
    if (_game_over) {
        if (action) {
            // Restart the game
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

        // Play action sound effect
        // Use ACTION_SOUND_FILES[0] when performing the last action of a level
        int action_sound_index = (_obstacles.size() == 1) ? 0 : _row;
        ISound *action_sound = _sound_engine->play2D(
            (SOUND_DIR + ACTION_SOUND_FILES[action_sound_index]).c_str(), false, true
        );
        action_sound->setVolume(ACTION_VOLUME);
        action_sound->setIsPaused(false);

        // Obstacles fade in when they appear, but when an action starts, we
        // cancel fade-in because we want to begin fading *out*
        for (auto &o : _obstacles) {
            o->setVolume(1);
        }
    }

    _time_since_action_start += dt;

    if (_action_in_progress) {
        const float action_progress = _time_since_action_start / ACTION_DURATION;
        if (action_progress >= 1) {
            finishAction();
            showTutorialMessage();
        } else {
            if (_obstacles.size()) {
                _obstacles[0]->setVolume(1 - action_progress);
            }
        }
    } else {
        // Fade obstacles in
        for (auto &o : _obstacles) {
            o->setVolume(std::min(1.f, o->getVolume() + dt / FADEIN_TIME));
        }
    }

    updateSafetyVolume();

    for (size_t i = 0; i < _obstacles.size(); ++i) {
        _obstacles[i]->step(dt, i);
    }

    // Lose when there is a collision with the first obstacle, or also with the second if we're
    // in a transition
    int collided_with = -1;
    if (std::abs(_obstacles[0]->getX()) < COLLISION_DISTANCE) {
        collided_with = 0;
    } else if (_action_in_progress
               && _obstacles.size() > 1
               && std::abs(_obstacles[1]->getX()) < COLLISION_DISTANCE) {
        collided_with = 1;
    }
    if (collided_with >= 0) {
        ISound *loss_sound = _sound_engine->play2D(
            (SOUND_DIR + LOSS_SOUND_FILES[_obstacles[collided_with]->getIndex() - 1]).c_str(),
            false, true
        );
        loss_sound->setVolume(LOSS_VOLUME);
        loss_sound->setIsPaused(false);

        lose();
    }
}

void Game::loadSounds() {
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
        auto *sound_source = _sound_engine->addSoundSourceFromFile(
            (SOUND_DIR + name).c_str(),
            irrklang::ESM_AUTO_DETECT,
            true // true = preload the sound
        );
        if (!sound_source) {
            throw std::runtime_error("Couldn't load sound source: " + name);
        }
        _sound_sources.push_back(sound_source);
    }
}

void Game::finishAction() {
    _action_in_progress = false;
    assert(!_obstacles.empty());
    _obstacles.erase(_obstacles.begin());

    if (_obstacles.empty()) {
        _level++;
        startNewLevel();
    } else {
        // In case we run out of row symbols, just use the last one
        int i = std::min(int32_t(ROW_SYMBOLS.size()) - 1, _row);
        std::cout << ROW_SYMBOLS[i] << std::endl;
    }
}

void Game::lose() {
    // Disable the tutorial once we've shown every message
    if (TUTORIAL_MESSAGES.upper_bound({_level, _row}) == TUTORIAL_MESSAGES.end()) {
        _tutorial = false;
    }
    _game_over = true;
    _obstacles.clear();
    _action_in_progress = false;
    _time_since_action_start = 0;
    _safety_sound->setVolume(0);

    std::cout << "You lost! You reached level " << _level << "." << std::endl;
    std::cout << "Press Q to quit, press ENTER to play again." << std::endl;
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

void Game::showTutorialMessage() {
    if (_tutorial) {
        if (TUTORIAL_MESSAGES.count({_level, _row})) {
            std::cout << TUTORIAL_MESSAGES.at({_level, _row}) << std::endl;
        }
    }
}