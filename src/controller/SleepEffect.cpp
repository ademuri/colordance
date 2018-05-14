#include "SleepEffect.hpp"
#include "LightController.hpp"

SleepEffect::SleepEffect(LightController *lightController,
                         ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SleepEffect::BeatDetected() {}

void SleepEffect::DoRun() {
  lightController->Set(lightIds[0], {hsv.h, hsv.s, hsv.v});
  lightController->Set(lightIds[2], {hsv.h + 120, hsv.s, hsv.v});
  lightController->Set(lightIds[4], {hsv.h + 240, hsv.s, hsv.v});
  hsv.h++;
  SleepMs(100);
}

void SleepEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kHue0:
      // hsv.h = paramController->Get(Params::kHue0);
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

void SleepEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLights(paramController, 1, 5)[0];
  // hsv.h = paramController->Get(Params::kHue0);

  TurnOffUnusedLights(oldLightIds, lightIds);
}
