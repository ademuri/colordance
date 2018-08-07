#include "Effect.hpp"
#include <algorithm>
#include <vector>

#ifndef ARDUINO
#include <cstdlib>

long random(long max) { return std::rand() % max; }
#endif

Effect::Effect(LightController *lightController,
               ParamController *paramController)
    : lightController(lightController), paramController(paramController) {}

void Effect::Run() {
  if (this->runAtMs < lightController->GetMs()) {
    DoRun();
    lightController->WriteLeds();
  } else {
    // If it's not time to advance the state, just make sure that the lights are
    // up to date with the state. This prevents flickering if the params have
    // changed and the effect is sleeping.
    SetLights();
  }
}

void Effect::SleepMs(long ms) { this->runAtMs = lightController->GetMs() + ms; }

void Effect::TurnOffUnusedLights(const std::vector<int16_t> &oldLightIds,
                                 const std::vector<int16_t> &currentLightIds) {
  std::vector<int16_t>::const_iterator it;
  for (it = oldLightIds.begin(); it != oldLightIds.end(); it++) {
    if (std::find(currentLightIds.begin(), currentLightIds.end(), *it) ==
        currentLightIds.end()) {
      // Light was picked before, and now it's not
      lightController->Set(*it, {0, 0, 0});
    }
  }
}

void Effect::ReloadParams() {
  for (auto iter : paramController->paramRangeMap) {
    ParamChanged(iter.first);
  }

  // This being called probably means we've been reset, so clear sleep.
  this->runAtMs = 0;
}
