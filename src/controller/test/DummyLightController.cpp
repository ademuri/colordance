#include "DummyLightController.hpp"

DummyLightController::DummyLightController(const int8_t rows,
                                           const int8_t cols) {
  numRows = rows;
  numCols = cols;

  int16_t counter = 1;

  for (int8_t i = 0; i < rows; i++) {
    std::vector<int16_t> row;
    for (int8_t j = 0; j < cols; j++) {
      row.push_back(counter++);
    }
    lightIds.push_back(row);
  }

  centerLightRow = rows / 2;
  centerLightCol = cols / 2;
}

long DummyLightController::GetMs() { return 1; }

void DummyLightController::Set(const int16_t lightId, HSV hsv) {
  lights[lightId] = hsv;
}

HSV DummyLightController::Get(const int16_t lightId) { return lights[lightId]; }
