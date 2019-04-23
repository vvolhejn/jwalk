# Jwalk

An audio-only game about avoiding sounds. It's a bit like [Frogger](https://www.youtube.com/watch?v=okm0VtF2gH8), but if the frog were blind.

## Installation

(Tested on macOS Mojave 10.14 and Linux Mint 17.3. Should work on Windows as well.)

Simply run `make` from this directory. This will download irrKlang, the library used for playing 3D sound, and then build the binary.

If everything works, the game's binary should be in `build/jwalk`. To play, run `cd build; ./jwalk -t`. The `-t` flag enables the tutorial.

## Known issues

- When using g++, version >=4.9 is required. If you have multiple versions and the default is too low, run something like `export CXX=g++-8`, replacing `g++-8` with your g++>=4.9 binary, remove `build/` and rerun `make`.
- `nanosleep` has low resolution on Linux (probably related to [this](https://gha.st/short-sleeps/)), so the "framerate" is lower than it should be.
- The sound sometimes crackles on Linux (ALSA sound driver). It seems to be a problem of the system or library because this happens in irrKlang's own examples (tested on example 2). The crackling typically goes away after a minute or so, and sometimes doesn't happen at all.
- There are some memory leaks according to Valgrind, but these seem to be due to irrKlang. They happen in irrKlang's own examples as well. [Possibly relevant forum entry here.](https://www.ambiera.com/forum.php?t=2166).

## Architecture

- `main.cpp` is the entry point, it contains the main loop and reads user input
- `Constants.h` contains values which are hardcoded, such as filenames or game parameters.
- The `Game` class contains (not surprisingly) information about the entire game.
- The `Obstacle` class represents one obstacle; in the Frogger analogy, this would be a single row or lane. It manages the obstacle's movement and sound
