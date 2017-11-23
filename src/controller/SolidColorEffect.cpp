#include "SolidColorEffect.hpp"
#include "LightController.hpp"

SolidColorEffect::SolidColorEffect(LightController *lightController,
                                   ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
}

void SolidColorEffect::DoRun() {
  // TODO: make this effect actually be a solid color, instead of a testing
  // ground

  lightController->Set(lightIds[0], hsv1);
  lightController->Set(lightIds[1], hsv2);
  lightController->Set(lightIds[2], hsv3);

  hsv1.h++;
  hsv2.h++;
  hsv3.h++;
}

void SolidColorEffect::BeatDetected() {
  hsv1.h += 60;
  hsv2.h += 60;
  hsv3.h += 60;
}

void SolidColorEffect::ChooseLights() {
  lightIds = lightController->GetLights(paramController, 1, 3)[0];
}
