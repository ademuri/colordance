#include "WS2812MatrixLightController.hpp"
#include <WProgram.h>
#include "../../src/color/Color.hpp"

WS2812MatrixLightController::WS2812MatrixLightController() {
  FastLED.addLeds<NEOPIXEL, kLedPin>(matrixLeds, kNumLeds);
  //FastLED.setBrightness(32);
  for (int i = 0; i < kNumLeds; i++) {
    matrixLeds[i] = CRGB{0, 0, 0};
  }
  FastLED.show();
  numRows = 16;
  numCols = 16;

  int16_t id = 0;
  for (int i = 0; i < numRows; i++) {
    std::vector<int16_t> row;
    for (int j = 0; j < numCols; j++) {
      row.push_back(id);

      id++;
    }
    lightIds.push_back(row);
  }
  digitalWrite(13, LOW);

  centerLightRow = numRows / 2;
  centerLightCol = numCols / 2;
}

void WS2812MatrixLightController::Set(const int16_t lightId, HSV hsv) {
    const struct RGB rgb = Color::toRGB(hsv);
    const int16_t row = (lightId / numCols);
    const int16_t col = (lightId % numCols);

    const int16_t col_width = kRawCols / numCols;
    const int16_t row_width = kRawRows / numRows;

    for (int i = 0; i < col_width; i++) {
      for (int j = 0; j < row_width; j++) {
        int16_t index = (row * row_width + j) * kRawCols + col * col_width + i;
        if ((index / kRawRows) % 2 == 1) {
          const int16_t start_of_row = (index / kRawCols) * kRawCols;
          index = start_of_row + kRawCols - (index - start_of_row) - 1;
        }
        matrixLeds[index] = CRGB{rgb.r, rgb.g, rgb.b};
      }
    }
}

unsigned long WS2812MatrixLightController::GetMs() { return millis(); }

void WS2812MatrixLightController::WriteLeds() {
  FastLED.show();
}

void WS2812MatrixLightController::SetButtonLights(bool status) {
}
