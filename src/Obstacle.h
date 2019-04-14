//
// Created by Vaclav Volhejn on 2019-03-10.
//

#ifndef JWALK_OBSTACLE_H
#define JWALK_OBSTACLE_H

#include <random>

#include <irrKlang.h>

class Obstacle {
    bool _free; // A free "obstacle" is one with no danger
    float _x;
    float _vx; // x velocity
    irrklang::ISound *_main_sound, *_warning_sound;
    float _volume = 1;
    int _index;

    void move(float dt, size_t row);

    float getTimeToReachEdge();

    float getTimeToReachCenter();

public:
    Obstacle(int index);

    Obstacle(irrklang::ISound *main_sound, irrklang::ISound *warning_sound,
             float x, float vx, int index);

    ~Obstacle();

    void step(float dt, size_t row);

    float getX() const;

    int getIndex() const;

    bool isFree() const;

    void setVolume(float volume);

    float getVolume();

    void randomizePosition(std::mt19937 &rng, int level);
};


#endif //JWALK_OBSTACLE_H
