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

// This is some stuff to fix issues with the Teensy library vector
// implementation where
// it throws warnings or fails to link if there is no __throw_bad_alloc()
// or __throw_length_error() functions.

namespace std {
void __throw_bad_alloc() {
  Serial.println("bad_alloc");
  while (1)
    ;
}
void __throw_length_error(char const *e) {
  Serial.print("length_error: ");
  Serial.println(e);
  while (1)
    ;
}
}

// Tuning constants
/** Fall asleep if not interacted with for this long. */
const unsigned long kNoInteractionSleepMs = 5 * 60 * 1000;
// const long kNoInteractionSleepMs = 5 * 1000;

// How long after no controls have been changed to randomly change the effect
// Note that this won't work as long as a knob controls the effect
const unsigned long kAutoEffectBaseMs = 60 * 1000;
const unsigned long kAutoEffectRandomMs = 30 * 1000;
// const unsigned long kAutoEffectBaseMs = 5 * 1000;
// const unsigned long kAutoEffectRandomMs = 2 * 1000;

extern "C" int main(void) {
  pinMode(13, OUTPUT);

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
      }
      effect->Run();
      ParamChanged changed = paramController->ScanForChanges(effect);
      if (changed != ParamChanged::kNone) {
        if (changed == ParamChanged::kChooseLights) {
          effect->ChooseLights();
        }
        sleepAt = millis() + kNoInteractionSleepMs;
        autoEffectAt =
            millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
      }

      if (millis() > sleepAt) {
        sleeping = true;
        lightController->Blackout();
        sleepEffect->ChooseLights();
      }

      if (millis() > autoEffectAt) {
        paramController->SetScaled(Params::kEffect, random(effects.size()), 0,
                                   effects.size() - 1);
        autoEffectAt =
            millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
        autoEffectTriggered = true;

        // Skip the delay to reduce the chance of flickering
        continue;
      }

      delay(1);
    }
  }
}
