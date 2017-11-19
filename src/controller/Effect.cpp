#include "Effect.hpp"

Effect::Effect(LightController *controller) : controller(controller) {
  runAtMs = 0;
}

void Effect::Run() {
  if (this->runAtMs < controller->GetMs()) {
    DoRun();
  }
}

void Effect::SleepMs(uint16_t ms) { this->runAtMs = controller->GetMs() + ms; }
