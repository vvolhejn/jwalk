//
// Created by Vaclav Volhejn on 2019-03-10.
//

#ifndef JWALK_GAME_H
#define JWALK_GAME_H

#include <memory>
#include <vector>

#include <irrKlang.h>
#include "Constants.h"
#include "Obstacle.h"

class Game {
    irrklang::ISoundEngine *_sound_engine;
    std::vector<irrklang::ISoundSource*> _sound_sources;
    std::vector<irrklang::ISound*> _warning_sounds;

    std::vector<std::unique_ptr<Obstacle>> _obstacles;

    void loadSounds();

public:
    Game(irrklang::ISoundEngine *sound_engine);

    ~Game();

    void step(float dt);
};

#endif //JWALK_GAME_H
