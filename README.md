# ColorDance

ColorDance is an art project that uses multiple colored lights pointed at a
screen to create colorful shadows.

## Structure

I'm loosely following this guide for project structure:
https://rix0r.nl/blog/2015/08/13/cmake-guide/

## Components

### Simulator

ColorDance will eventually have (rather large) hardware. To ease development,
the Simulator simulates colored lights pointed at a screen.

TODO: create an actual interface for the simulator.

## Dependencies

### Google Test

Build and install Google Test from [the
repo](https://github.com/google/googletest), and CMake will pick it up.

### Ogre3D

Build and install Ogre3D from [the repo](https://github.com/OGRECave/ogre), and
CMake will pick it up. ColorDance uses Ogre 1.10.
