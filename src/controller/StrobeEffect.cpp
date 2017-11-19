#include "StrobeEffect.hpp"
#include "LightController.hpp"

StrobeEffect::StrobeEffect(LightController *controller) : Effect(controller) {}

void StrobeEffect::DoRun() {
  if (on) {
    controller->Set(Lights::STAGE_LEFT, hsv1);
    controller->Set(Lights::STAGE_RIGHT, {0, 0, 0});
    hsv1.h += 3;
  } else {
    controller->Set(Lights::STAGE_LEFT, {0, 0, 0});
    controller->Set(Lights::STAGE_RIGHT, hsv2);
    hsv2.h += 3;
  }

  on = !on;
  SleepMs(100);
}
