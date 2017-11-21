#include "Effect.hpp"

Effect::Effect(LightController *lightController,
               ParamController *paramController)
    : lightController(lightController), paramController(paramController) {
  runAtMs = 0;
}

void Effect::Run() {
  if (this->runAtMs < lightController->GetMs()) {
    DoRun();
  }
}

void Effect::SleepMs(uint16_t ms) {
  this->runAtMs = lightController->GetMs() + ms;
}
