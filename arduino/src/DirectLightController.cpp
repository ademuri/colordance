#include "WProgram.h"
#include "DirectLightController.hpp"
#include "../../src/Color.hpp"


DirectLightController::DirectLightController() {
  lightIds = {};
  int currentId = 1;
  for (int rows = 0; rows < 5; row++) {
    std::vector<uint16_t> row = {};
    for (int cols = 0; cols < 5; cols++) {
      row.push_back(currentId);
      currentId += 3;
    }
    lightIds.push_back(row);
  }
  dmx.begin();
  dmx.setPacketSize(80);
}

void DirectLightController::Set(const uint16_t lightId, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  dmx.set(lightId * 3, rgb.r);
  dmx.set(lightId * 3 + 1, rgb.g);
  dmx.set(lightId * 3 + 2, rgb.b);
}

uint16_t DirectLightController::GetMs() {
  return millis();
}
