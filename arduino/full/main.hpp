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

// Tracking for when usage was last detected
unsigned long screenMotionAt = 0;
unsigned long controlMotionAt = 0;
unsigned long controlUsedAt = 0;

// When to flash the controls, to hint to the users that they should play with
// the control surface.
unsigned long flashControlsAt = 0;
unsigned long flashControlsEndAt = 0;
bool flashingControls = false;

const unsigned long flashControlsEvery = 30 * 1000;
const unsigned long flashControlsDuration = 1 * 1000;

// Motion sensors. Note that these are connected to analog inputs which, unlike
// ATMEGA-based Arduino devices, cannot be read using digitalRead.
// Pointed towards the screen
static const int kScreenMotionPin = A10;
// Pointed towards the controls
static const int kControlMotionPin = A11;

// Since the motion pins are analog, we need to specify a threshold. The max
// value is 1023.
static const int kMotionThresh = 800;

/** Fall asleep if not interacted with for this long. */
static const unsigned long kNoInteractionSleepMs = 5 * 60 * 1000;
// const long kNoInteractionSleepMs = 5 * 1000;

// How long after no controls have been changed to randomly change the effect
// Note that this won't work as long as a knob controls the effect
static const unsigned long kAutoEffectBaseMs = 60 * 1000;
static const unsigned long kAutoEffectRandomMs = 30 * 1000;
// const unsigned long kAutoEffectBaseMs = 5 * 1000;
// const unsigned long kAutoEffectRandomMs = 2 * 1000;

#endif
