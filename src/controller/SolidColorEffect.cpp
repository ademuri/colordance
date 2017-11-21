#include "SolidColorEffect.hpp"
#include "LightController.hpp"

SolidColorEffect::SolidColorEffect(LightController *lightController,
                                   ParamController *paramController)
    : Effect(lightController, paramController) {}

void SolidColorEffect::DoRun() {
  // TODO: make this effect actually be a solid color, instead of a testing
  // ground
  lightController->Set(Lights::TOP, hsv1);
  lightController->Set(Lights::CENTER, hsv2);
  lightController->Set(Lights::BOTTOM, hsv3);

  hsv1.h++;
  hsv2.h++;
  hsv3.h++;
}

void SolidColorEffect::BeatDetected() {
  hsv1.h += 60;
  hsv2.h += 60;
  hsv3.h += 60;
}
