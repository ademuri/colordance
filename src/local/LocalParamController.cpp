#include "LocalParamController.hpp"

LocalParamController::LocalParamController() : ParamController() {}

int16_t LocalParamController::Get(Params param) { return params[param]; }

void LocalParamController::Set(Params param, int16_t val) {
  params[param] = val;
}

bool LocalParamController::Boost() { return boost; }

void LocalParamController::SetBoost(bool boost) { this->boost = boost; }
