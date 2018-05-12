#include "DirectLightController.hpp"
#include "../../src/color/Color.hpp"
#include "WProgram.h"

DirectLightController::DirectLightController() {
  int currentId = 1;
  for (int row = 0; row < 5; row++) {
    std::vector<uint16_t> rowIds;
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
  centerLightRow = 3;
  centerLightCol = 3;
}

void DirectLightController::Set(const uint16_t lightId, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  dmx.set(lightId, rgb.r);
  dmx.set(lightId + 1, rgb.g);
  dmx.set(lightId + 2, rgb.b);
}

long DirectLightController::GetMs() { return millis(); }
