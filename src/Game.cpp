//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Game.h"

#include <iostream>
#include <string>

#include "Obstacle.h"

//using irrklang::ISoundEngine;
using irrklang::vec3df;

Game::~Game() {
    for (auto* sound : _warning_sounds) {
        sound->drop();
    }
    if (_sound_engine) {
        _sound_engine->drop();
    }
}


Game::Game(irrklang::ISoundEngine *sound_engine)
    : _sound_engine(sound_engine) {
    loadSounds();
//    _sound_engine->setDopplerEffectParameters(10, 1.0);
    _sound_engine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));

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

        _obstacles.push_back(std::make_unique<Obstacle>(music, -10, 20 * (i + 1), 1 * (i + 1)));
    }
}

void Game::step(float dt) {
    for (size_t i = 0; i < _obstacles.size(); ++i) {
        _obstacles[i]->step(dt, i);
        if (i < WARNING_SOUND_FILES.size()) {
            if (abs(_obstacles[i]->getX()) < WARNING_DISTANCE) {
                vec3df pos3d(_obstacles[i]->getX(), 0, DISTANCE_BETWEEN_ROWS);
                _warning_sounds[i]->setPosition(pos3d);
                _warning_sounds[i]->setVolume(1 - abs(_obstacles[i]->getX()) / WARNING_DISTANCE);
            } else {
                _warning_sounds[i]->setVolume(0);
            }
        }
    }
}

void Game::loadSounds(){
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
