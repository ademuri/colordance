#include "SleepEffect.hpp"
#include <math.h>
#include "LightController.hpp"

SleepEffect::SleepEffect(LightController *lightController,
                         ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SleepEffect::BeatDetected() {}

void SleepEffect::DoRun() {
  if (entice) {
    if (lightController->GetMs() > enticeEndAtMs) {
      entice = false;
      enticeAtMs = lightController->GetMs() + kEnticeEveryMs +
                   random(kEnticeEveryRandom);
    } else {
      HSV bright = {hsv.h, hsv.s,
                    127 + 127 * sin(lightController->GetMs() / 100.0)};
      lightController->Set(lightIds[0], bright);
      lightController->Set(lightIds[2], bright);
      lightController->Set(lightIds[4], bright);
      hsv.h++;
      SleepMs(10);
    }
  } else {
    if (lightController->GetMs() > enticeAtMs) {
      entice = true;
      enticeEndAtMs = lightController->GetMs() + kEnticeDurationMs +
                      random(kEnticeDurationRandom);
    } else {
      lightController->Set(lightIds[0], {hsv.h, hsv.s, hsv.v});
      lightController->Set(lightIds[2], {hsv.h + 120, hsv.s, hsv.v});
      lightController->Set(lightIds[4], {hsv.h + 240, hsv.s, hsv.v});
      hsv.h++;
      SleepMs(100);
    }
  }
}

void SleepEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
    case Params::kTempo:
    case Params::kOrientation:
    case Params::kParam1:
    case Params::kParam2:
    case Params::kKnob:
      break;
  }
}

void SleepEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLights(paramController, 1, 5)[0];

  TurnOffUnusedLights(oldLightIds, lightIds);

  enticeAtMs =
      lightController->GetMs() + kEnticeEveryMs + random(kEnticeEveryRandom);
}
