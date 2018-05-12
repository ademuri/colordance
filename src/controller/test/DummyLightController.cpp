#include "DummyLightController.hpp"

DummyLightController::DummyLightController(const uint8_t rows,
                                           const uint8_t cols) {
  numRows = rows;
  numCols = cols;

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

long DummyLightController::GetMs() { return 1; }

void DummyLightController::Set(const uint16_t lightId, HSV hsv) {
  lights[lightId] = hsv;
}

HSV DummyLightController::Get(const uint16_t lightId) {
  return lights[lightId];
}
