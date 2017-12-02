#include "ParamController.hpp"

ParamController::ParamController() {
  paramRangeMap[Params::kHue0] = 359;
  paramRangeMap[Params::kTempo] = 255;
  paramRangeMap[Params::kWidth] = 255;
  paramRangeMap[Params::kPan] = 255;
  paramRangeMap[Params::kTilt] = 255;
}

uint16_t ParamController::GetScaled(Params param, uint16_t min, uint16_t max) {
  // TODO: technically going from uint16 to int16 could result in clipping, but
  // it probably won't happen (param values shouldn't be near the limit of
  // int16).
  // Use a signed int so that we can have max < min
  const int16_t expectedRange = max - min;
  const uint16_t actualRange = paramRangeMap[param];
  return min + (Get(param) * expectedRange) / actualRange;
}

uint16_t ParamController::WrapParam(Params param, uint16_t val) {
  return val % (paramRangeMap[param] + 1);
}

void ParamController::SetScaled(Params param, int16_t val, int16_t min,
                                int16_t max) {
  uint16_t scaledVal = (val - min) * paramRangeMap[param] / (max - min);
  Set(param, scaledVal);
}
