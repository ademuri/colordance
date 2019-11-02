#include "MiniParamController.hpp"
#include "WProgram.h"

MiniParamController::MiniParamController() : ParamController() {
  for (auto pair : params) {
    params[pair.first] = 0;
  }
}

int16_t MiniParamController::Get(Params param) { return params[param]; }

void MiniParamController::Set(Params param, int16_t val) {
  params[param] = val;
}
