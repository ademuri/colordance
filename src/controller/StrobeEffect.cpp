#include "StrobeEffect.hpp"
#include "LightController.hpp"

StrobeEffect::StrobeEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
  hsv.h = paramController->Get(Params::kHue0);
}

void StrobeEffect::DoRun() {
  if (currentLight >= lightIds.size()) {
    currentLight = 0;
  }

  HSV adjustedHsv = hsv;
  adjustedHsv.h += hueAdjust * currentLight;
  lightController->Set(lightIds[currentLight], adjustedHsv);

  if (currentLight == 0) {
    lightController->Set(lightIds[lightIds.size() - 1], {0, 0, 0});
  } else {
    lightController->Set(lightIds[currentLight - 1], {0, 0, 0});
  }

  currentLight++;
  hsv.h += 10;
  SleepMs(paramController->GetScaled(Params::kTempo, 1000, 75));
}

void StrobeEffect::ChooseLights() {
  // TODO: extract this logic into the LightController
  const uint16_t numLights = paramController->GetScaled(Params::kWidth, 2, 9);
  lightIds = lightController->GetLights(paramController, 1, numLights)[0];
  hueAdjust = 320 / numLights;
}

void StrobeEffect::BeatDetected() {
  hsv.h += 30;
  DoRun();
}
