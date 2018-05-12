#include "ParamController.hpp"

ParamController::ParamController() {}

int16_t ParamController::GetScaled(Params param, int16_t min, int16_t max) {
  int16_t expectedRange = max - min;
  const int16_t actualRange = paramRangeMap[param];

  if (expectedRange == 0) {
    return min;
  }

  // If expectedRange is even, then the number of bins returned is odd. In that
  // case, add one to expectedRange so that the range is centered.
  if (expectedRange % 2 == 0) {
    if (expectedRange > 0) {
      expectedRange++;
    } else {
      expectedRange--;
    }
  }

  // return min + ((Get(param) + offset) * expectedRange) / actualRange;
  int16_t val = min + Get(param) * expectedRange / actualRange;
  if (max > min) {
    if (val > max) {
      val = max;
    } else if (val < min) {
      val = min;
    }
  } else if (min > max) {
    if (val < max) {
      val = max;
    } else if (val > min) {
      val = min;
    }
  }
  return val;
}

int16_t ParamController::WrapParam(Params param, int16_t val) {
  return val % (paramRangeMap[param] + 1);
}

void ParamController::SetScaled(Params param, int16_t val, int16_t min,
                                int16_t max) {
  int16_t scaledVal = (val - min) * paramRangeMap[param] / (max - min);
  Set(param, scaledVal);
}
