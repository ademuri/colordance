#include "ColorShiftAndStrobeEffect.hpp"
#include "LightController.hpp"

ColorShiftAndStrobeEffect::ColorShiftAndStrobeEffect(
    LightController *lightController, ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
  hsvAdvance = 0;
}

void ColorShiftAndStrobeEffect::DoRun() {
  for (uint16_t i = 0; i < lightIds.size(); i++) {
    if (lightsOn) {
      HSV hsv = {hsv.h + i * hsvShift, hsv.s, hsv.v};
      lightController->Set(lightIds[i], hsv);
    } else {
      HSV hsv = {0, 0, 0};
      lightController->Set(lightIds[i], hsv);
    }
  }

  hsv.h += hsvAdvance;

  SleepMs(paramController->GetScaled(Params::kTempo, 1000, 75));
  lightsOn = !lightsOn;
}

void ColorShiftAndStrobeEffect::BeatDetected() { hsv.h += 60; }

void ColorShiftAndStrobeEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kTempo:
      // hsvAdvance = paramController->GetScaled(Params::kTempo, 0, 10);
      hsvAdvance = 0;
      break;

    // TODO: handle other cases
    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
      break;
  }
}

void ColorShiftAndStrobeEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<uint16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLightsFromParams(paramController);
  // hsvShift = 180 / lightIds.size();
  hsvShift = 2;
  hsv.v = 100 + (310 / lightIds.size());

  TurnOffUnusedLights(oldLightIds, lightIds);
}
