#include "ColorShiftEffect.hpp"
#include "LightController.hpp"

ColorShiftEffect::ColorShiftEffect(LightController *lightController,
                                   ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
}

void ColorShiftEffect::DoRun() {
  for (uint16_t i = 0; i < lightIds.size(); i++) {
    lightController->Set(lightIds[i], {hsv.h + i * hsvShift, hsv.s, hsv.v});
  }

  hsv.h++;
}

void ColorShiftEffect::BeatDetected() { hsv.h += 60; }

void ColorShiftEffect::ChooseLights() {
  for (uint16_t i = 0; i < lightIds.size(); i++) {
    lightController->Set(lightIds[i], {0, 0, 0});
  }

  const uint16_t numLights =
      paramController->GetScaled(Params::kWidth, 2, lightController->numCols);
  lightIds = lightController->GetLights(paramController, 1, numLights)[0];
  hsvShift = 360 / numLights;
  hsv.v = 100 + (310 / numLights);
}
