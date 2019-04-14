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


class Game {
    irrklang::ISoundEngine *_sound_engine;
    std::vector<irrklang::ISoundSource *> _sound_sources;
    std::vector<std::unique_ptr<Obstacle>> _obstacles;
    // Plays when there is no obstacle in the current row
    irrklang::ISound *_safety_sound;
    float _time_since_action_start;
    bool _action_in_progress;
    int _level;
    // The row is equal to the number of actions performed in the current level
    int _row;
    std::mt19937 _rng;
    // True iff the game has ended and we are waiting if the user wants to play again
    bool _game_over;

    // Load the necessary sound files into memory
    void loadSounds();

    // Called when an action ends; removes the obstacle which was "left behind"
    void finishAction();

    void startNewLevel();

    // Enter the "game over" state
    void lose();

    // Fades the safety sound in and out as appropriate
    void updateSafetyVolume();

public:
    Game(irrklang::ISoundEngine *sound_engine);

    ~Game();

    void step(float dt, bool action);
};

#endif //JWALK_GAME_H
