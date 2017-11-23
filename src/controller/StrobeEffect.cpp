#include "StrobeEffect.hpp"
#include "LightController.hpp"

StrobeEffect::StrobeEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
  hsv1.h = paramController->Get(Params::kHue0);
  hsv2.h = hsv1.h + 60;
  hsv2.s = 127;
}

void StrobeEffect::DoRun() {
  if (on) {
    lightController->Set(lightIds[0], hsv1);
    lightController->Set(lightIds[1], {0, 0, 0});
    // hsv1.h += 3;
  } else {
    lightController->Set(lightIds[0], {0, 0, 0});
    lightController->Set(lightIds[1], hsv2);
    hsv2.h += 10;
  }

  on = !on;
  SleepMs(paramController->GetScaled(Params::kTempo, 1000, 75));
}

void StrobeEffect::ChooseLights() {
  lightIds = lightController->GetLights(paramController, 1, 2)[0];
}

void StrobeEffect::BeatDetected() {
  hsv1.h += 30;
  hsv2.h = hsv1.h + 60;
  DoRun();
}
