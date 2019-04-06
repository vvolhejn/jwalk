//
// Created by Vaclav Volhejn on 2019-03-10.
//

#ifndef JWALK_OBSTACLE_H
#define JWALK_OBSTACLE_H

#include <irrKlang.h>

class Obstacle {
    bool _free; // A free "obstacle" is one with no danger
    float _x;
    float _vx; // x velocity
    irrklang::ISound *_sound;
    float _fadeout = 0;

    void move(float dt, size_t row);

    float getTimeToReachEdge();

public:
    Obstacle();

    Obstacle(irrklang::ISound *sound, float x, float vx);

    ~Obstacle();

    void step(float dt, size_t row);

    float getX() const;

    void setFadeout(float fadeout);
};


#endif //JWALK_OBSTACLE_H
