#include "DirectParamController.hpp"
#include <Encoder.h>
#include "WProgram.h"

DirectParamController::DirectParamController() : ParamController() {}

uint16_t DirectParamController::Get(Params param) { return params[param]; }

void DirectParamController::Set(Params param, uint16_t val) {
  params[param] = val;
}

ParamChanged DirectParamController::ScanForChanges(Effect *effect) {
  bool paramChanged = false;

  for (std::map<const Params, const int>::const_iterator iter =
           potParamMap.begin();
       iter != potParamMap.end(); iter++) {
    const int readValue = analogRead(iter->second) >> 2;
    if (readValue != params[iter->first]) {
      paramChanged = true;
      params[iter->first] = readValue;
      effect->ParamChanged(iter->first);
    }
  }

  for (std::map<const Params, Encoder *>::iterator iter =
           encoderParamMap.begin();
       iter != encoderParamMap.end(); iter++) {
    const int readValue = iter->second->read();
    if (readValue != params[iter->first]) {
      paramChanged = true;
      params[iter->first] = readValue;
      effect->ParamChanged(iter->first);
    }
  }

  // TODO: return the correct value depending on the param
  if (paramChanged) {
    return ParamChanged::kChooseLights;
  } else {
    return ParamChanged::kNone;
  }
}
