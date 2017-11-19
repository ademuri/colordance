#include "SolidColorEffect.hpp"
#include "LightController.hpp"

SolidColorEffect::SolidColorEffect(LightController *controller)
    : Effect(controller) {}

void SolidColorEffect::DoRun() {
  // TODO: make this effect actually be a solid color, instead of a testing
  // ground
  controller->Set(Lights::TOP, hsv1);
  controller->Set(Lights::CENTER, hsv2);
  controller->Set(Lights::BOTTOM, hsv3);

  hsv1.h++;
  hsv2.h++;
  hsv3.h++;
}
