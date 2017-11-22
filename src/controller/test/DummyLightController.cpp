#include "DummyLightController.hpp"

DummyLightController::DummyLightController(const uint8_t rows,
                                           const uint8_t cols) {
  uint16_t counter = 1;

  for (uint8_t i = 0; i < rows; i++) {
    std::vector<uint16_t> row;
    for (uint8_t j = 0; j < cols; j++) {
      row.push_back(counter++);
    }
    lightIds.push_back(row);
  }

  centerLightRow = rows / 2;
  centerLightCol = cols / 2;
}

void DummyLightController::Set(const Lights light, HSV hsv) {}

uint16_t DummyLightController::GetMs() { return 0; }
