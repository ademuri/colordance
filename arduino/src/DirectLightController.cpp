#include "DirectLightController.hpp"
#include <WProgram.h>
#include "../../src/color/Color.hpp"
#include "../libraries/SPI/SPI.h"

DirectLightController::DirectLightController() {
  int currentId = 1;
  for (int row = 0; row < 5; row++) {
    std::vector<int16_t> rowIds;
    for (int col = 0; col < 5; col++) {
      rowIds.push_back(currentId);
      currentId += 3;
    }
    lightIds.push_back(rowIds);
  }
  dmx.begin();
  // TODO: this needs to be some magic minimum value for DMX to work (?)
  dmx.setPacketSize(192);

  numRows = 5;
  numCols = 5;
  centerLightRow = 2;
  centerLightCol = 2;

  // Set up SPI for the serial LEDs (APA102-like)
  SPI1.setMOSI(0);
  SPI1.setSCK(32);
  for (int i = 0; i < kNumLeds; i++) {
    leds.push_back({0, 0, 0});
  }
}

void DirectLightController::Set(const int16_t lightId, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  dmx.set(lightId, rgb.r);
  dmx.set(lightId + 1, rgb.g);
  dmx.set(lightId + 2, rgb.b);
}

long DirectLightController::GetMs() { return millis(); }

void DirectLightController::WriteLeds() {
  // Write out to the APA102-like LEDs. See here for the protocol:
  // https://cpldcpu.wordpress.com/2014/11/30/understanding-the-apa102-superled/

  SPI1.begin();
  // 12 MHz
  SPI1.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));

  // Start frame
  SPI1.transfer(0);
  SPI1.transfer(0);
  SPI1.transfer(0);
  SPI1.transfer(0);

  // Write the LEDs
  for (auto rgb : leds) {
    // Brightness
    SPI1.transfer(0xFF);
    SPI1.transfer(rgb.b);
    SPI1.transfer(rgb.g);
    SPI1.transfer(rgb.r);
  }

  // Reset frame (needed for SK9822)
  SPI1.transfer(0);
  SPI1.transfer(0);
  SPI1.transfer(0);
  SPI1.transfer(0);

  // End frame - needs to be at least 1/2 bit per LED
  for (int i = 0; i < (kNumLeds + 1) / 16; i++) {
    SPI1.transfer(0);
  }

  SPI1.endTransaction();
  SPI1.end();
}
