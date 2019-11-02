#ifndef __WS2812_MATRIX_LIGHT_CONTROLLER_HPP__
#define __WS2812_MATRIX_LIGHT_CONTROLLER_HPP__

#include "../../src/color/Color.hpp"
#include "../../src/controller/LightController.hpp"
#include "FastLED/FastLED.h"

class WS2812MatrixLightController : public LightController {
 public:
  WS2812MatrixLightController();

  void Set(const int16_t lightId, HSV hsv) override;
  unsigned long GetMs() override;
  void WriteLeds() override;
  void SetButtonLights(bool status) override;

 private:
  /** The number of serial LEDs. */
  static const int kNumLeds = 256;

  /** The physical size of the matrix. */ 
  static const int kRawRows = 16;
  static const int kRawCols = 16;

  static const int kLedPin = 0;

  CRGB matrixLeds[kNumLeds];
};

#endif
