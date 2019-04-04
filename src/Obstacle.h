//
// Created by Vaclav Volhejn on 2019-03-10.
//

#ifndef JWALK_OBSTACLE_H
#define JWALK_OBSTACLE_H

#include <irrKlang.h>

class Obstacle {
    float _x;

public:
    float getX() const;

private:
    float _vx; // x velocity
    irrklang::ISound* _sound;

public:
    Obstacle(irrklang::ISound* sound, float x, float y, float vx);
    ~Obstacle();
    void step(float dt, size_t row);
};


#endif //JWALK_OBSTACLE_H
