// This example will show how to play sounds in 3D space using irrKlang.
// An mp3 file file be played in 3D space and moved around the user and a
// sound will be played at a random 3D position every time the user presses
// a key.

// For this example, we need some function to sleep for some seconds,
// so we include the platform specific sleep functions here. This is
// only need for demo purposes and has nothing to do with sound output.
// include console I/O methods (conio.h for windows, our wrapper in linux)
#if defined(WIN32)
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#else
#include "../lib/irrKlang-64bit-1.6.0/examples/common/conio.h"
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

void sleepSeconds(float t) {
    usleep(useconds_t(t * 1e6));
}

int main(int argc, const char** argv) {
    ISoundEngine* engine = createIrrKlangDevice();

    if (!engine) {
        return 0; // error starting up the engine
    }

    Game game(engine);

    auto time_prev = steady_clock::now();

    while(true) {
        auto time_now = steady_clock::now();
        float secs = duration_cast<nanoseconds>(time_now - time_prev).count() / 1e9;
        time_prev = time_now;

        if (secs> SLEEP_TIME * 2) {
            std::cerr << "Warning: slept for " << secs
                << " instead of " << SLEEP_TIME << std::endl;
        }

        game.step(secs);

        sleepSeconds(SLEEP_TIME);
//        sleepSomeTime();

        if (kbhit()) {
            int key = getch();
            if (key == 'q') {
                break;
            }
        }
    }
    return 0;
}
