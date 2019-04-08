//
// Created by Vaclav Volhejn on 2019-03-16.
//

#ifndef JWALK_CONSTANTS_H
#define JWALK_CONSTANTS_H

#include <vector>
#include <string>

//////////////////// Sound files ////////////////////
const std::string SOUND_DIR = "../media/";
const std::vector<std::string> OBSTACLE_SOUND_FILES = {
    "a4_8.wav",
    "e4_6.wav",
    "h2_4.wav",

};
const std::vector<std::string> WARNING_SOUND_FILES = {
    "a4_8_dist.wav",
    "e4_6_dist.wav",
    "h2_4_dist.wav",
};

//////////////////// Game constants ////////////////////
const int32_t SEED = 123;
const size_t N_OBSTACLES = 3;
const float COLLISION_DISTANCE = 1;
const float WARNING_DISTANCE = 7; // How close must an obstacle be to start playing a warning sound?
const float DISTANCE_BETWEEN_ROWS = 10;
const float ACTION_DURATION = 0.8; // How long it takes to advance to the next rows
const float EDGE_DISTANCE = 20; // Position of the edge at which sounds return from the other side
// Begin fading out an obstacle when it will reach the edge in this time
const float EDGE_FADEOUT_TIME = 1;

#endif //JWALK_CONSTANTS_H
