#include "DirectParamController.hpp"

DirectParamController::DirectParamController() : ParamController() {}

uint16_t DirectParamController::Get(Params param) { return params[param]; }

void DirectParamController::Set(Params param, uint16_t val) {
  params[param] = val;
}
