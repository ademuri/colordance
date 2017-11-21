#include "DummyParamController.hpp"

DummyParamController::DummyParamController() : ParamController() {}

uint16_t DummyParamController::Get(Params param) { return params[param]; }

void DummyParamController::Set(Params param, uint16_t val) {
  params[param] = val;
}
