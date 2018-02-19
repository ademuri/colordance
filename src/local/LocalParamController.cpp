#include "LocalParamController.hpp"

LocalParamController::LocalParamController() : ParamController() {}

uint16_t LocalParamController::Get(Params param) { return params[param]; }

void LocalParamController::Set(Params param, uint16_t val) {
  params[param] = val;
}

