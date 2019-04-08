//
// Created by Vaclav Volhejn on 2019-03-10.
//

#ifndef JWALK_GAME_H
#define JWALK_GAME_H

#include <memory>
#include <vector>
#include <random>

#include <irrKlang.h>
#include "Constants.h"
#include "Obstacle.h"

// Value of _time_since_action_start if no action is taking place
//const float NO_ACTION = -1;

class Game {
    irrklang::ISoundEngine *_sound_engine;
    std::vector<irrklang::ISoundSource *> _sound_sources;
    std::vector<std::unique_ptr<Obstacle>> _obstacles;
    irrklang::ISound *_safety_sound;
    float _time_since_action_start;
    bool _action_in_progress;
    int _level;
    int _row;
    std::mt19937 _rng;
    bool _game_over;

    void loadSounds();

    void finishAction();

    void startNewLevel();

    void lose();

    void updateSafetyVolume();

public:
    Game(irrklang::ISoundEngine *sound_engine);

    ~Game();

    void step(float dt, bool action);
};

#endif //JWALK_GAME_H
