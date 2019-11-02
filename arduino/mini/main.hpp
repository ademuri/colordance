#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <string>
#include "WProgram.h"

/** Fall asleep if not interacted with for this long. */
static const unsigned long kNoInteractionSleepMs = 5 * 60 * 1000;
// const long kNoInteractionSleepMs = 5 * 1000;

// How long after no controls have been changed to randomly change the effect
// Note that this won't work as long as a knob controls the effect
static const unsigned long kAutoEffectBaseMs = 10 * 1000;
static const unsigned long kAutoEffectRandomMs = 0 * 1000;
// const unsigned long kAutoEffectBaseMs = 5 * 1000;
// const unsigned long kAutoEffectRandomMs = 2 * 1000;

#endif
