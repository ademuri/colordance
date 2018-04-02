#include "SolidColorEffect.hpp"
#include <iostream>
#include "LightController.hpp"

SolidColorEffect::SolidColorEffect(LightController *lightController,
                                   ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SolidColorEffect::BeatDetected() {}

void SolidColorEffect::DoRun() {
  for (uint16_t i = 0; i < lightIds.size(); i++) {
    HSV hsv = {hsv.h + i * hsvShift, hsv.s, hsv.v};
    lightController->Set(lightIds[i], hsv);
  }
}

void SolidColorEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kHue0:
      hsv.h = paramController->Get(Params::kHue0);
      break;

    // TODO: handle other cases
    case Params::kHue1:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
    case Params::kTempo:
      break;
  }
}

void SolidColorEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<uint16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLightsFromParams(paramController);
  hsvShift = 360 / lightIds.size();
  hsv.h = paramController->Get(Params::kHue0);
  hsv.v = 100 + (155 / lightIds.size());

  TurnOffUnusedLights(oldLightIds, lightIds);
}
