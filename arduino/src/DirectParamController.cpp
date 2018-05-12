#include "DirectParamController.hpp"
#include <Encoder.h>
#include "WProgram.h"

DirectParamController::DirectParamController() : ParamController() {}

int16_t DirectParamController::Get(Params param) { return params[param]; }

void DirectParamController::Set(Params param, int16_t val) {
  params[param] = val;
}

ParamChanged DirectParamController::ScanForChanges(Effect *effect) {
  bool paramChanged = false;
  bool chooseLights = false;

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

  for (std::map<const Params, Encoder *const>::iterator iter =
           encoderParamMap.begin();
       iter != encoderParamMap.end(); iter++) {
    const int16_t paramRange = paramRangeMap[iter->first];
    const int readValue =
        (iter->second->read() % paramRange + paramRange) % paramRange;
    if (readValue != params[iter->first]) {
      paramChanged = true;
      params[iter->first] = readValue;
      effect->ParamChanged(iter->first);

      if (!chooseLights &&
          chooseLightParams.find(iter->first) != chooseLightParams.end()) {
        chooseLights = true;
      }
    }
  }

  // TODO: return the correct value depending on the param
  if (paramChanged) {
    return chooseLights ? ParamChanged::kChooseLights : ParamChanged::kOther;
  } else {
    return ParamChanged::kNone;
  }
}
