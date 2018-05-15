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

    case Params::kHue1:
      hsv.s =
          100 + std::abs(paramController->GetScaled(Params::kHue1, -155, 155));
      break;

    // TODO: handle other cases
    case Params::kHue0:
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
  hsvShift = 360 / lightIds.size();

  TurnOffUnusedLights(oldLightIds, lightIds);
}
