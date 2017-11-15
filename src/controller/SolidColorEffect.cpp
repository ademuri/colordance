#include "SolidColorEffect.hpp"
#include "LightController.hpp"

SolidColorEffect::SolidColorEffect(LightController *controller)
    : Effect(controller) {}

void SolidColorEffect::run() {
  controller->Set(Lights::STAGE_LEFT, {HUE_RED, 255, 255});
  controller->Set(Lights::CENTER, {HUE_GREEN, 255, 255});
  controller->Set(Lights::STAGE_RIGHT, {HUE_BLUE, 255, 255});
}
