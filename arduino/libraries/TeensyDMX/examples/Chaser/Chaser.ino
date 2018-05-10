/*
 * Demonstrates outputting DMX by chasing values across all the channels.
 *
 * (c) 2017 Shawn Silverman
 */

#include "TeensyDMX.h"

// Interval between channel value changes. A value of 20ms was chosen
// so that all we cycle through all 512 channels every
// 512 * 0.02s = 10.24 seconds.
constexpr unsigned long kChaseInterval = 20;

namespace teensydmx = ::qindesign::teensydmx;

// Create the DMX transmitter on Serial1.
teensydmx::Sender dmxTx{Serial1};

// The current channel outputting a value.
int channel;

void setup() {
  Serial.begin(9600);
  delay(2000);  // Instead of while (!Serial), doesn't seem to work on Teensy
  Serial.println("Starting.");

  dmxTx.begin();

  channel = 1;
  dmxTx.set(channel, 255);
}

void loop() {
  static elapsedMillis lastChase = 0;
  if (lastChase >= kChaseInterval) {
    // Set the current channel to zero, advance it, and then set the
    // new channel to 255
    dmxTx.set(channel, 0);
    channel++;
    if (channel > 512) {
      channel = 1;
    }
    dmxTx.set(channel, 255);

    lastChase = 0;
  }
}
