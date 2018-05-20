// This defines the logging function, so it needs to be first
#include "main.hpp"

#include "../../src/controller/BounceEffect.hpp"
#include "../../src/controller/CircleStrobeEffect.hpp"
#include "../../src/controller/ColorShiftAndStrobeEffect.hpp"
#include "../../src/controller/ColorShiftEffect.hpp"
#include "../../src/controller/SleepEffect.hpp"
#include "../../src/controller/SolidColorEffect.hpp"
#include "../../src/controller/StrobeEffect.hpp"
#include "../../src/controller/ThreeColorEffect.hpp"
#include "../../src/controller/UnevenSwitchStrobeEffect.hpp"
#include "DirectLightController.hpp"
#include "DirectParamController.hpp"
#include "WProgram.h"

template <typename... Args>
void log(const std::string &format, Args... args) {
  static char buffer[kLogBufferSize];
  // Print the time and then the caller's message
  snprintf(buffer, kLogBufferSize, "%lu, ", millis());
  Serial5.print(buffer);

  snprintf(buffer, kLogBufferSize, format.c_str(), args...);
  Serial5.println(buffer);
}

// Tuning constants
/** Fall asleep if not interacted with for this long. */
const unsigned long kNoInteractionSleepMs = 5 * 60 * 1000;
// const long kNoInteractionSleepMs = 5 * 1000;

// How long after no controls have been changed to randomly change the effect
// Note that this won't work as long as a knob controls the effect
// const unsigned long kAutoEffectBaseMs = 60 * 1000;
// const unsigned long kAutoEffectRandomMs = 30 * 1000;
const unsigned long kAutoEffectBaseMs = 5 * 1000;
const unsigned long kAutoEffectRandomMs = 2 * 1000;

extern "C" int main(void) {
  pinMode(13, OUTPUT);
  Serial5.begin(115200);

  log("Hello, world!");

  DirectLightController *lightController = new DirectLightController();
  DirectParamController *paramController = new DirectParamController();

  std::vector<Effect *> effects = {
      new ColorShiftEffect(lightController, paramController),
      new CircleStrobeEffect(lightController, paramController),
      new BounceEffect(lightController, paramController),
      // TODO: fully parameterize these and clean them up, then uncomment
      // new ThreeColorEffect(lightController, paramController),
      // new SolidColorEffect(lightController, paramController),
      // new StrobeEffect(lightController, paramController),
      // new UnevenSwitchStrobeEffect(lightController, paramController),
      // new ColorShiftAndStrobeEffect(lightController, paramController),
  };
  paramController->setNumEffects(effects.size());

  SleepEffect *sleepEffect = new SleepEffect(lightController, paramController);

  Effect *effect = effects[0];
  paramController->ScanForChanges(effect);
  effect->ChooseLights();
  effect->ReloadParams();
  int effectIndex = 0;
  int prevEffectIndex = effectIndex;

  unsigned long sleepAt = millis() + kNoInteractionSleepMs;
  bool sleeping = false;

  unsigned long autoEffectAt =
      millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
  bool autoEffectTriggered = false;
  // Census variable that keeps track of whether the params were automatically
  // generated.
  bool usingAutoEffect = false;

  while (1) {
    if (sleeping) {
      sleepEffect->Run();
      if (paramController->ScanForChanges(effect) != ParamChanged::kNone) {
        sleeping = false;
        sleepAt = millis() + kNoInteractionSleepMs;
        lightController->Blackout();
        // TODO: check motion sensors too
      }

    } else {
      // Awake
      effectIndex = paramController->getEffectIndex();
      if (effectIndex != prevEffectIndex || autoEffectTriggered) {
        effect = effects[effectIndex];
        prevEffectIndex = effectIndex;
        lightController->Blackout();

        if (autoEffectTriggered) {
          autoEffectTriggered = false;
          effect->RandomizeParams();
        }

        effect->ChooseLights();
        effect->ReloadParams();
      } else if (paramController->getRandomize()) {
        // If we haven't already switched effects and the randomize button is
        // pressed, randomize the params
        autoEffectAt =
            millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
        sleepAt = millis() + kNoInteractionSleepMs;

        effect->RandomizeParams();
        effect->ChooseLights();
        effect->ReloadParams();
      }

      ParamChanged changed = paramController->ScanForChanges(effect);
      if (changed != ParamChanged::kNone) {
        if (changed == ParamChanged::kChooseLights) {
          effect->ChooseLights();
        }
        sleepAt = millis() + kNoInteractionSleepMs;
        autoEffectAt =
            millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
        usingAutoEffect = false;
      }

      if (millis() > sleepAt) {
        sleeping = true;
        lightController->Blackout();
        sleepEffect->ChooseLights();
      }
      effect->Run();

      if (millis() > autoEffectAt) {
        paramController->setEffectIndex(random(effects.size()));
        autoEffectAt =
            millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
        autoEffectTriggered = true;
        usingAutoEffect = true;

        // Skip the delay to reduce the chance of flickering
        continue;
      }

      delay(1);
    }

    // Census: log certain values every so often. Log CSV-like as "name, value"
    if (millis() > censusLogAt) {
      // Note: this seems to take about 1-2ms.
      log("census, effect=%d, sleeping=%d, usingAutoEffect=%s", effectIndex,
          sleeping, usingAutoEffect);
      censusLogAt = millis() + kCensusLogMs;
    }
  }
}
