#include "DummyLightController.hpp"

DummyLightController::DummyLightController() {
  lightIds = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  centerLightRow = 1;
  centerLightCol = 1;
}

void DummyLightController::Set(const Lights light, HSV hsv) {}

uint16_t DummyLightController::GetMs() { return 0; }
