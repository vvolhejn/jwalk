//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Obstacle.h"

#include <iostream>
#include <algorithm>

#include "Constants.h"

using irrklang::vec3df;

Obstacle::Obstacle(irrklang::ISound *sound, float x, float vx)
    : _sound(sound), _x(x), _vx(vx), _free(false) {
}

Obstacle::Obstacle()
    : _free(true), _x(EDGE_DISTANCE), _sound(nullptr) {
}

Obstacle::~Obstacle() {
    if (_sound) {
        _sound->stop();
        _sound->drop();
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

void Obstacle::step(float dt, size_t row) {
    if (_free) {
        return;
    }
    move(dt, row);

    float volume = 1 - _fadeout;
    // Fade out when near the edge to prevent the sound from "jumping"
    volume *= std::min(1.f, getTimeToReachEdge() / EDGE_FADEOUT_TIME);
    _sound->setVolume(volume);
}

float Obstacle::getX() const {
    return _x;
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
    _sound->setPosition(pos3d);
}
