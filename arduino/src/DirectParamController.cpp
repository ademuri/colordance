#include "DirectParamController.hpp"
#include <Encoder.h>
#include "WProgram.h"

DirectParamController::DirectParamController() : ParamController() {
  for (auto pair : potParamMap) {
    // Note: this will be overriden the first time through ScanForChanges
    potValueMap[pair.first] = 0;
  }
}

int16_t DirectParamController::Get(Params param) { return params[param]; }

void DirectParamController::Set(Params param, int16_t val) {
  params[param] = val;

  // If this is an encoder-controller param, reset the encoder to this value so
  // that when the user rotates the encoder, it doesn't 'jump'.
  if (encoderParamMap.find(param) != encoderParamMap.end()) {
    encoderParamMap[param]->write(val);
  }
}

ParamChanged DirectParamController::ScanForChanges(Effect *effect) {
  bool paramChanged = false;
  bool chooseLights = false;

  for (std::map<const Params, const int>::const_iterator iter =
           potParamMap.begin();
       iter != potParamMap.end(); iter++) {
    const int readValue = analogRead(iter->second) >> 2;
    if (abs(readValue - potValueMap[iter->first]) > 1) {
      paramChanged = true;
      potValueMap[iter->first] = readValue;
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
