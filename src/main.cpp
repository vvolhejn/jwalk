//
// Created by Vaclav Volhejn on 2019-03-10.
//

#if defined(WIN32)
#include <windows.h>
#include <conio.h>
#else

#include "conio.h"

#endif

// Lets start: include the irrKlang headers and other input/output stuff
// needed to print and get user input from the console. And as exlained
// in the first tutorial, we use the namespace irr and audio and
// link to the irrKlang.dll file.
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <iostream>
#include <unistd.h>

#include <irrKlang.h>

#include "Game.h"

using namespace irrklang;
using namespace std::chrono;

const float SLEEP_TIME = 0.01f;
const int ENTER_KEY = 10;

void sleepSeconds(float t) {
    uint32_t s = t;
    timespec tm;
    tm.tv_sec = s;
    tm.tv_nsec = uint32_t((t - s) * 1e9);

    nanosleep(&tm, NULL);
}

int main(int argc, const char **argv) {
    if ((argc > 1) && (std::string(argv[1]) == "-h")) {
        std::cout << "Jwalk. Run with -t for a tutorial." << std::endl;
        return 0;
    }
    bool tutorial = (argc > 1) && (std::string(argv[1]) == "-t");

    ISoundEngine *engine = createIrrKlangDevice();

    if (!engine) {
        // There was an error starting up the engine
        return 0;
    }

    Game game(engine, tutorial);
    auto time_prev = steady_clock::now();
    bool action = false;

    while (true) {
        auto time_now = steady_clock::now();
        float secs = duration_cast<nanoseconds>(time_now - time_prev).count() / 1e9;
        time_prev = time_now;

#ifdef DEBUG
        if (secs > SLEEP_TIME * 2) {
            std::cerr << "Warning: slept for " << secs
                << " instead of " << SLEEP_TIME << std::endl;
        }
#endif

        game.step(secs, action);
        action = false;

        sleepSeconds(SLEEP_TIME);

        if (kbhit()) {
            int key = getch();
            if (key == ENTER_KEY) {
                action = true;
            } else if (key == 'q') {
                break;
            }
        }
    }

    return 0;
}
