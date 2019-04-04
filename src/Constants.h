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
    "rhodes1.wav",
    "rhodes2.wav",
    "rhodes3.wav",
};
const std::vector<std::string> WARNING_SOUND_FILES = {
    "warning1.wav",
    "warning2.wav",
};

//////////////////// Game constants ////////////////////
const size_t N_OBSTACLES = 3;
// How close must an obstacle be to start playing a warning sound?
const float WARNING_DISTANCE = 5;
const float DISTANCE_BETWEEN_ROWS = 10;


#endif //JWALK_CONSTANTS_H
