# ColorDance

ColorDance is an art project that uses multiple colored lights pointed at a
screen to create colorful shadows.

## Structure

I'm loosely following this guide for project structure:
https://rix0r.nl/blog/2015/08/13/cmake-guide/

## Building

ColorDance uses CMake. To build everything, first make a 'build' folder in the
root directory. cd to that directory, and run::

```bash
cmake ../src
make
```

This will build everything, including the simulator and the tests.

To run the all of the tests, just run `make test` from the build dir. You can
also run the binaries directly.

## Components

### Simulator

ColorDance will eventually have (rather large) hardware. To ease development,
the Simulator simulates colored lights pointed at a screen.

To run the simulator, run `simulator/Simulator` from the build directory. You
may need to make it executable (e.g. with chmod +x) first.

TODO: create an actual interface for the simulator.

## Dependencies

### Google Test

Build and install Google Test from [the
repo](https://github.com/google/googletest), and CMake will pick it up.

### Ogre3D

Build and install Ogre3D from [the repo](https://github.com/OGRECave/ogre), and
CMake will pick it up. ColorDance uses Ogre 1.10.
