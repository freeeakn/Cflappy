# Flappy Bird Clone

A simple Flappy Bird clone written in C using SDL2.

## Prerequisites

You need to have SDL2 installed on your system:

### macOS

```bash
brew install sdl2
```

### Ubuntu/Debian

```bash
sudo apt-get install libsdl2-dev
```

## Building

To build the game, simply run:

```bash
make
```

## Running

After building, run the game with:

```bash
./flappy
```

## Controls

- Press SPACE to make the bird jump
- Close the window to quit the game

## Game Rules

- Navigate the bird through the pipes
- Each successfully passed pipe gives you 1 point
- Hitting a pipe or the ground ends the game
- Your final score will be displayed when the game ends
