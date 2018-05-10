#include "WProgram.h"
#include "DirectParamController.hpp"

DirectParamController::DirectParamController() : ParamController() {}

uint16_t DirectParamController::Get(Params param) {
  switch (param) {
    case Params::kWidth:
      return analogRead(kWidthPin) >> 2;
      break;

    default:
      return params[param];
  }
}

void DirectParamController::Set(Params param, uint16_t val) {
  params[param] = val;
}
