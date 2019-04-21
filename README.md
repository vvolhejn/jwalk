# Jwalk

An audio-only game about avoiding sounds.

Tested on macOS Mojave 10.14 and Linux Mint 17.3.

## Installation

Simply run `make`. This will:

- Download IrrKlang, the library used for playing 3D sound
- (TODO) Download the necessary sound files
- Build the binary

If everything works, the game's binary should be in `build/jwalk`.
Run the binary from the `build` directory to play.

## Known problems

- `nanosleep` has low resolution on Linux (probably related to [this](https://gha.st/short-sleeps/)), so the "framerate" is lower
- The sound sometimes crackles on Linux (ALSA). It seems to be a problem of the system or library because this happens in irrKlang's own examples (tested on example 2). The crackling typically goes away after a minute or so.
