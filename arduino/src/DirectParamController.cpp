#include "DirectParamController.hpp"
#include "WProgram.h"

DirectParamController::DirectParamController() : ParamController() {}

uint16_t DirectParamController::Get(Params param) { return params[param]; }

void DirectParamController::Set(Params param, uint16_t val) {
  params[param] = val;
}

ParamChanged DirectParamController::ScanForChanges(Effect *effect) {
  bool paramChanged = false;

  std::map<const Params, const int>::const_iterator iter;
  for (iter = potParamMap.begin(); iter != potParamMap.end(); iter++) {
    const int readValue = analogRead(iter->second) >> 2;
    if (readValue != params[iter->first]) {
      paramChanged = true;
      params[iter->first] = readValue;
      effect->ParamChanged(iter->first);
    }
  }

  // TODO: return the correct value depending on the param
  return ParamChanged::kChooseLights;
}
