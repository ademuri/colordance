#include "DummyParamController.hpp"

DummyParamController::DummyParamController() : ParamController() {}

int16_t DummyParamController::Get(Params param) { return params[param]; }

void DummyParamController::Set(Params param, int16_t val) {
  params[param] = val;
}

bool DummyParamController::Boost() { return boost; }

void DummyParamController::SetBoost(bool boost) { this->boost = boost; }
