#include "../../src/controller/BounceEffect.hpp"
#include "../../src/controller/CircleStrobeEffect.hpp"
#include "../../src/controller/ColorShiftAndStrobeEffect.hpp"
#include "../../src/controller/ColorShiftEffect.hpp"
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
const long kNoInteractionSleepMs = 5 * 60 * 1000;

extern "C" int main(void) {
  pinMode(13, OUTPUT);

  DirectLightController *lightController = new DirectLightController();
  DirectParamController *paramController = new DirectParamController();
  paramController->Set(Params::kHue0, 120);
  paramController->Set(Params::kTempo, 254);
  paramController->Set(Params::kWidth, 255);
  paramController->Set(Params::kPan, ParamController::kPanNeutral);
  paramController->Set(Params::kTilt, ParamController::kTiltNeutral);

  std::vector<Effect *> effects = {
      new ThreeColorEffect(lightController, paramController),
      new ColorShiftEffect(lightController, paramController),
      new SolidColorEffect(lightController, paramController),
      new CircleStrobeEffect(lightController, paramController),
      new BounceEffect(lightController, paramController),
      new StrobeEffect(lightController, paramController),
      new UnevenSwitchStrobeEffect(lightController, paramController),
      new ColorShiftAndStrobeEffect(lightController, paramController),
  };

  Effect *effect = effects[0];
  effect->ChooseLights();
  int effectIndex = 0;
  int prevEffectIndex = effectIndex;

  long sleepAt = millis() + kNoInteractionSleepMs;
  bool sleeping = false;

  while (1) {
    if (sleeping) {
      if (paramController->ScanForChanges(effect) != ParamChanged::kNone) {
        sleeping = false;
        sleepAt = millis() + kNoInteractionSleepMs;
        // TODO: check motion sensors too
      }
    } else {
      effectIndex =
          paramController->GetScaled(Params::kEffect, 0, effects.size() - 1);
      if (effectIndex != prevEffectIndex) {
        // TODO: make effect reload all params
        effect = effects[effectIndex];
        prevEffectIndex = effectIndex;
        lightController->Blackout();
        effect->ChooseLights();
      }
      effect->Run();

      if (paramController->ScanForChanges(effect) != ParamChanged::kNone) {
        effect->ChooseLights();
        sleepAt = millis() + kNoInteractionSleepMs;
      }
      if (millis() > sleepAt) {
        sleeping = true;
        // TODO: make a sleep pattern
        lightController->Blackout();
      }
      delay(1);
    }
  }
}
