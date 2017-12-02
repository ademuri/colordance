#include "Effect.hpp"
#include <algorithm>
#include <vector>

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

void Effect::TurnOffUnusedLights(const std::vector<uint16_t> &oldLightIds,
                                 const std::vector<uint16_t> &currentLightIds) {
  std::vector<uint16_t>::const_iterator it;
  for (it = oldLightIds.begin(); it != oldLightIds.end(); it++) {
    if (std::find(currentLightIds.begin(), currentLightIds.end(), *it) ==
        currentLightIds.end()) {
      // Light was picked before, and now it's not
      lightController->Set(*it, {0, 0, 0});
    }
  }
}
