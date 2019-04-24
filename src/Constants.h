//
// Created by Vaclav Volhejn on 2019-03-16.
//

#ifndef JWALK_CONSTANTS_H
#define JWALK_CONSTANTS_H

#include <vector>
#include <string>
#include <map>

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

const std::vector<std::string> LOSS_SOUND_FILES = {
    "a4_dist.wav",
    "e4_dist.wav",
    "h2_dist.wav",
};

const std::vector<std::string> ACTION_SOUND_FILES = {
    "bea.wav",
    "a1.wav",
    "e1.wav",
    "h0.wav",
};

const std::string SAFETY_SOUND_FILE = "safety.wav";

//////////////////// Game constants ////////////////////
const size_t N_OBSTACLES = 3;
// The first level with (i+1) obstacles is MLFS[i]
const std::vector<int> MIN_LEVEL_FOR_OBSTACLE = {1, 5, 15};

// The maximum value of |x| of an obstacle considered a collision
const float COLLISION_DISTANCE = 1;
const float DISTANCE_BETWEEN_ROWS = 10;
const float ACTION_DURATION = 0.8; // How long it takes to advance to the next rows
const float EDGE_DISTANCE = 15; // Position of the edge at which sounds return from the other side
// Symbols printed when an action finishes
//const std::vector<std::string> ROW_SYMBOLS = {"𝄞", "♬", "♫", "♪", "♩"};
const std::vector<std::string> ROW_SYMBOLS = {"Level", ".", "..", "...", "...."};

//////////////////// Sound constants ////////////////////
// Begin fading in the warning sound this many seconds before it reaches the center
const float WARNING_TIME_BEFORE = 4;
// Finish fading out the warning sound this many seconds after it reaches the center
const float WARNING_TIME_AFTER = 2;
// Begin fading out an obstacle when it will reach the edge in this time
const float EDGE_FADEOUT_TIME = 2;
const float ACTION_VOLUME = 0.2;
const float SAFETY_VOLUME = 0.1;
const float LOSS_VOLUME = 0.13;
// When a new obstacle appears (beginning of a level), it should be completely faded in after
const float FADEIN_TIME = 0.5;

//////////////////// Tutorial////////////////////
// In level i and row j, show TUTORIAL_MESSAGES[{i, j}]
const std::map<std::pair<int32_t, int32_t>, std::string> TUTORIAL_MESSAGES = {
    {{0, 0},
        "This is Jwalk. You will need stereo sound.\n"
        "You are trying to move forward while avoiding dangerous sounds. "
        "Luckily, you're now hearing the safety sound; it means you are not in danger now. "
        "Press ENTER to move forward."
    },
    {{1, 0},
        "Welcome to level 1. The beeping tone is an obstacle. It moves from side to side and when "
        "it touches you, you lose. Press ENTER to enter the obstacle's row."
    },
    {{1, 1},
        "The obstacle can hit you now. Try it by doing nothing. Or press ENTER to go on."
    },
    {{2, 0},
        "Congratulations on completing level 1!"
    },
    {{MIN_LEVEL_FOR_OBSTACLE[1], 0},
        "This is where it gets tricky: there are now two obstacles. They are in separate rows. "
        "The new one is further away."
    },
    {{MIN_LEVEL_FOR_OBSTACLE[1], 1},
        "The higher tone can hit you now."
    },
    {{MIN_LEVEL_FOR_OBSTACLE[1], 2},
        "And now you've advanced to the row of the lower tone."
    },
    {{MIN_LEVEL_FOR_OBSTACLE[1]+1, 0},
        "Good job! That's all there is to it. Have fun!"
    },
};

#endif //JWALK_CONSTANTS_H
