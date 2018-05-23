#include "ColorShiftEffect.hpp"
#include <cmath>
#include "LightController.hpp"

ColorShiftEffect::ColorShiftEffect(LightController *lightController,
                                   ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
}

void ColorShiftEffect::DoRun() {
  for (int16_t i = 0; i < lightIds.size(); i++) {
    lightController->Set(lightIds[i], {hsv.h + i * hsvShift, hsv.s, hsv.v});
  }

  hsv.h += hsvAdvance;
  SleepMs(5);
}

void ColorShiftEffect::BeatDetected() { hsv.h += 60; }

void ColorShiftEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kTempo:
      hsvAdvance = paramController->GetScaled(Params::kTempo, 1, 8);
      break;

    case Params::kParam1:
      hsvShift = paramController->GetScaled(Params::kParam1, 30,
                                            360 / lightIds.size());
      break;

    case Params::kParam2:
      hsv.s = paramController->GetScaled(Params::kParam2, 100, 255);
      break;

    // TODO: handle other cases
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
      break;
  }
}

void ColorShiftEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLightsFromParams(paramController);
  hsvShift =
      paramController->GetScaled(Params::kParam1, 30, 360 / lightIds.size());

  TurnOffUnusedLights(oldLightIds, lightIds);
}

void ColorShiftEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(60), 0, 100);
  paramController->SetScaled(Params::kWidth, 1 + random(4), 0, 4);
  paramController->SetScaled(Params::kPan, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
  paramController->SetScaled(Params::kParam1, 100 + random(156), 100, 255);
  paramController->SetScaled(Params::kParam2, 100 + random(156), 100, 255);
#endif
}
