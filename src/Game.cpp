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
        irrklang::ISound *main_sound = _sound_engine->play3D(
            (SOUND_DIR + OBSTACLE_SOUND_FILES[i]).c_str(),
            vec3df(0, 0, 0),
            true, false, true
        );

        if (!main_sound) {
            throw std::runtime_error("Couldn't load sound: " +
                                     std::string(OBSTACLE_SOUND_FILES[i]));
        }

        irrklang::ISound *warning_sound = _sound_engine->play3D(
            (SOUND_DIR + WARNING_SOUND_FILES[i]).c_str(),
            vec3df(0, 0, 0),
            true, false, true
        );

        if (!warning_sound) {
            throw std::runtime_error("Couldn't load sound: " +
                                     std::string(WARNING_SOUND_FILES[i]));
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

        _obstacles.push_back(std::make_unique<Obstacle>(main_sound, warning_sound, x, vx));
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
    for (const std::string &name : OBSTACLE_SOUND_FILES) {
        auto *sound_source = loadSoundSource(name);
        _sound_sources.push_back(sound_source);
    }

    for (const std::string &name : WARNING_SOUND_FILES) {
        auto *sound_source = loadSoundSource(name);
        _sound_sources.push_back(sound_source);
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

    std::cout << "You lost! You reached level " << _level << "." << std::endl;
    std::cout << "Press Q to quit, press Enter to play again." << std::endl;
}