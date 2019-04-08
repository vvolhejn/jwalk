//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Obstacle.h"

#include <iostream>
#include <algorithm>

#include "Constants.h"

using irrklang::vec3df;
using irrklang::ISound;

Obstacle::Obstacle(ISound *main_sound, ISound *warning_sound, float x, float vx, int index)
    : _main_sound(main_sound), _warning_sound(warning_sound),
      _x(x), _vx(vx), _free(false), _index(index) {
}

Obstacle::Obstacle(int index)
    : _free(true), _x(EDGE_DISTANCE), _main_sound(nullptr), _warning_sound(nullptr), _index(index) {
    if (_main_sound) {
        _main_sound->setVolume(0);
    }
    if (_warning_sound) {
        _warning_sound->setVolume(0);
    }
}

Obstacle::~Obstacle() {
    if (_main_sound) {
        _main_sound->stop();
        _main_sound->drop();
    }
    if (_warning_sound) {
        _warning_sound->stop();
        _warning_sound->drop();
    }
}

float Obstacle::getTimeToReachEdge() {
    float vx = _vx;
    float x = _x;
    if (vx < 0) {
        x = -x;
        vx = -vx;
    }
    return (EDGE_DISTANCE - x) / vx;
}

float Obstacle::getTimeToReachCenter() {
    // May also return negative values; -0.5 means the obstacle was in the center 0.5s ago
    float vx = _vx;
    float x = _x;
    if (vx < 0) {
        x = -x;
        vx = -vx;
    }
    return - x / vx;
}

void Obstacle::step(float dt, size_t row) {
    if (_free) {
        return;
    }
    move(dt, row);

    float main_volume = 1 - _fadeout;
    // Fade out when near the edge to prevent the sound from "jumping"
    main_volume *= std::min(1.f, getTimeToReachEdge() / EDGE_FADEOUT_TIME);
    _main_sound->setVolume(main_volume);

    float warning_volume = 1 - _fadeout;
    float time_to_center = getTimeToReachCenter();
    if (time_to_center > 0) {
        warning_volume *= std::max(0.f, 1 - time_to_center / WARNING_TIME_BEFORE);
    } else {
        // The obstacle has passed the center.
        warning_volume *= std::max(0.f, 1 + time_to_center / WARNING_TIME_AFTER);
    }

    _warning_sound->setVolume(warning_volume);
}

int Obstacle::getIndex() const {
    return _index;
}

float Obstacle::getX() const {
    return _x;
}

bool Obstacle::isFree() const {
    return _free;
}

void Obstacle::setFadeout(float fadeout) {
    _fadeout = std::min(1.f, std::max(0.f, fadeout));
}

void Obstacle::move(float dt, size_t row) {
    _x += _vx * dt;
    // Loop to the other side if needed
    if (_vx > 0 && _x > EDGE_DISTANCE) {
        _x = -EDGE_DISTANCE;
    } else if (_vx < 0 && _x < -EDGE_DISTANCE) {
        _x = EDGE_DISTANCE;
    }
    float y = DISTANCE_BETWEEN_ROWS * (row + 1);
    vec3df pos3d(_x, 0, y);
    _main_sound->setPosition(pos3d);
    _warning_sound->setPosition(pos3d);
}
