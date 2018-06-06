#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <string>
#include "WProgram.h"

// This will be the max log message length. The Teensy 3.5 has 192kB of RAM, so
// no reason to make this too small.
const int kLogBufferSize = 500;

template <typename... Args>
extern void log(const std::string& format, Args... args);

/** How frequently to do a census log, in milliseconds. */
// const unsigned long kCensusLogMs = 10000;
const unsigned long kCensusLogMs = 5 * 1000;

unsigned long censusLogAt = 0;
unsigned long lastCensusAt = 0;

unsigned long screenMotionAt = 0;
unsigned long controlMotionAt = 0;
unsigned long controlUsedAt = 0;

// Motion sensors. Note that these are connected to analog inputs which, unlike
// ATMEGA-based Arduino devices, cannot be read using digitalRead.
// Pointed towards the screen
static const int kScreenMotionPin = A10;
// Pointed towards the controls
static const int kControlMotionPin = A11;

// Since the motion pins are analog, we need to specify a threshold. The max
// value is 1023.
static const int kMotionThresh = 500;

#endif
