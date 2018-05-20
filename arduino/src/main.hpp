#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <string>

// This will be the max log message length. The Teensy 3.5 has 192kB of RAM, so
// no reason to make this too small.
const int kLogBufferSize = 500;

template <typename... Args>
extern void log(const std::string& format, Args... args);

/** How frequently to do a census log, in milliseconds. */
const unsigned long kCensusLogMs = 10000;

unsigned long censusLogAt = 0;

#endif
