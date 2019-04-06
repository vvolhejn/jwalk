//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Game.h"

#include <iostream>
#include <string>
#include <algorithm>

#include "Obstacle.h"

//using irrklang::ISoundEngine;
using irrklang::vec3df;

Game::~Game() {
    for (auto *sound : _warning_sounds) {
        sound->drop();
    }
    if (_sound_engine) {
        _sound_engine->drop();
    }
}


Game::Game(irrklang::ISoundEngine *sound_engine)
    : _sound_engine(sound_engine), _time_since_action_start(NO_ACTION),
      _level(1), _game_over(false) {
//    _rng = std::mt19937(SEED);
    _rng = std::mt19937(time(0));

    loadSounds();
//    _sound_engine->setDopplerEffectParameters(10, 1.0);
    _sound_engine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));

    startNewLevel();
}

void Game::startNewLevel() {
    assert(_obstacles.empty());
    std::cerr << "Starting new level" << std::endl;

    // Start with a free obstacle
    _obstacles.push_back(std::make_unique<Obstacle>());

    for (int i = 0; i < N_OBSTACLES; ++i) {
        irrklang::ISound *music = _sound_engine->play3D(
            _sound_sources[i],
            vec3df(0, 0, 0),
            true, false, true
        );

        if (!music) {
            throw std::runtime_error("Couldn't load sound: " +
                                     std::string(_sound_sources[i]->getName()));
        }

        std::uniform_real_distribution<double> x_distribution(
            EDGE_DISTANCE * 0.5, EDGE_DISTANCE * 0.75);
        std::uniform_real_distribution<double> vx_distribution(-1, -3);
        float x = x_distribution(_rng);
        float vx = vx_distribution(_rng);

        if (_rng() % 2) {
            x = -x;
            vx = -vx;
        }

        _obstacles.push_back(std::make_unique<Obstacle>(music, x, vx));
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
    if (action && _time_since_action_start == NO_ACTION) {
        _time_since_action_start = 0;
    }

    float action_progress = 0;

    if (_time_since_action_start != NO_ACTION) {
        action_progress = _time_since_action_start / ACTION_DURATION;
        _time_since_action_start += dt;
        // TODO: smoother transition
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

    for (size_t i = 0; i < _obstacles.size(); ++i) {
        _obstacles[i]->step(dt, i);

        // Play warnings
        if (i < WARNING_SOUND_FILES.size()) {
            if (abs(_obstacles[i]->getX()) < WARNING_DISTANCE) {
                vec3df pos3d(_obstacles[i]->getX(), 0, DISTANCE_BETWEEN_ROWS);
                _warning_sounds[i]->setPosition(pos3d);
                float volume = 1 - abs(_obstacles[i]->getX()) / WARNING_DISTANCE;
                if (i == 0) {
                    // Fade out the warning of the obstacle which is about to disappear
                    volume *= 1 - action_progress;
                }
                _warning_sounds[i]->setVolume(volume);
            } else {
                _warning_sounds[i]->setVolume(0);
            }
        }
    }

    // In case there are more warnings than obstacles, disable the extra warnings
    for (size_t i = _obstacles.size(); i < _warning_sounds.size(); i++) {
        _warning_sounds[i]->setVolume(0);
    }

    // Lose when there is a collision with the first obstacle, or also with the second if we're
    // in a transition
    if (std::abs(_obstacles[0]->getX()) < COLLISION_DISTANCE
        || (_time_since_action_start != NO_ACTION
            && _obstacles.size() > 1
            && std::abs(_obstacles[1]->getX()) < COLLISION_DISTANCE)) {
        lose();
    }
}

void Game::loadSounds() {
    // Obstacles
    for (const std::string &name : OBSTACLE_SOUND_FILES) {
        auto *sound_source = _sound_engine->addSoundSourceFromFile(
            (SOUND_DIR + name).c_str(),
            irrklang::ESM_AUTO_DETECT,
            true // preload the sound
        );
        if (!sound_source) {
            throw std::runtime_error("Couldn't load sound source: " + name);
        }
        _sound_sources.push_back(sound_source);
    }

    // Warnings
    for (const std::string &name : WARNING_SOUND_FILES) {
        // start paused
        irrklang::ISound *sound = _sound_engine->play3D(
            (SOUND_DIR + name).c_str(),
            vec3df(0, 0, 10),
            true, true, true
        );
        if (!sound) {
            throw std::runtime_error("Couldn't load sound: " + name);
        }
        sound->setVolume(0);
        sound->setIsPaused(false);
        _warning_sounds.push_back(sound);
    }
}

void Game::finishAction() {
    _time_since_action_start = NO_ACTION;
    std::cout << "action finished" << std::endl;
    assert(!_obstacles.empty());
    _obstacles.erase(_obstacles.begin());
}

void Game::lose() {
    _game_over = true;
    _obstacles.clear();
    for (size_t i = 0; i < _warning_sounds.size(); i++) {
        _warning_sounds[i]->setVolume(0);
    }

    std::cout << "You lost! You reached level " << _level << "." << std::endl;
    std::cout << "Press Q to quit, press Enter to play again." << std::endl;
}