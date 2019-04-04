//
// Created by Vaclav Volhejn on 2019-03-10.
//

#include "Obstacle.h"

#include <iostream>

#include "Constants.h"

using irrklang::vec3df;

Obstacle::Obstacle(irrklang::ISound *sound, float x, float y, float vx)
    : _sound(sound), _x(x), _vx(vx) {
}

Obstacle::~Obstacle() {
    if (_sound) {
        _sound->drop();
    }
}

void Obstacle::step(float dt, size_t row) {
    _x += _vx * dt;
//    std::cout << _y << std::endl;
    float y = DISTANCE_BETWEEN_ROWS * (row + 1);
    vec3df pos3d(_x, 0, y);
    _sound->setPosition(pos3d);
}

float Obstacle::getX() const {
    return _x;
}
