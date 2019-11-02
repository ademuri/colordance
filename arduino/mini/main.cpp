// This defines the logging function, so it needs to be first
#include "main.hpp"

#include "../../src/controller/BounceEffect.hpp"
#include "../../src/controller/CircleStrobeEffect.hpp"
#include "../../src/controller/ColorShiftAndStrobeEffect.hpp"
#include "../../src/controller/ColorShiftEffect.hpp"
#include "../../src/controller/SleepEffect.hpp"
#include "../../src/controller/SolidColorEffect.hpp"
#include "../../src/controller/SquareEffect.hpp"
#include "../../src/controller/SquareStrobeEffect.hpp"
#include "../../src/controller/StrobeEffect.hpp"
#include "../../src/controller/ThreeColorEffect.hpp"
#include "../../src/controller/UnevenSwitchStrobeEffect.hpp"
#include "../libraries/DS1307RTC/DS1307RTC.h"
#include "../libraries/Time/TimeLib.h"
#include "WS2812MatrixLightController.hpp"
#include "MiniParamController.hpp"
#include "WProgram.h"

extern "C" int main(void) {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(20);

  WS2812MatrixLightController *lightController = new WS2812MatrixLightController();
  MiniParamController *paramController = new MiniParamController();
  paramController->SetScaled(Params::kWidth, 2, 0, 2);

  std::vector<Effect *> effects = {
      new ColorShiftEffect(lightController, paramController,
                           ColorShiftMode::straight),
      new ColorShiftEffect(lightController, paramController,
                           ColorShiftMode::ring),
      new BounceEffect(lightController, paramController),
      new SquareEffect(lightController, paramController),
      new SquareStrobeEffect(lightController, paramController),
      new CircleStrobeEffect(lightController, paramController),
      // TODO: fully parameterize these and clean them up, then uncomment
      // new ThreeColorEffect(lightController, paramController),
      // new SolidColorEffect(lightController, paramController),
      // new StrobeEffect(lightController, paramController),
      // new UnevenSwitchStrobeEffect(lightController, paramController),
      // new ColorShiftAndStrobeEffect(lightController, paramController),
  };

  // Clear any accidentally on lights from the constructors
  lightController->Blackout();
  Effect *effect = effects[0];
  effect->ChooseLights();
  effect->ReloadParams();
  int effectIndex = 0;
  int prevEffectIndex = effectIndex;

  unsigned long autoEffectAt =
      millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
  bool autoEffectTriggered = false;


  effect = new ColorShiftEffect(lightController, paramController, ColorShiftMode::straight);
  while (1) {
    if (effectIndex != prevEffectIndex || autoEffectTriggered) {
      effect = effects[effectIndex];
      prevEffectIndex = effectIndex;
      lightController->Blackout();
      lightController->WriteLeds();

      if (autoEffectTriggered) {
        autoEffectTriggered = false;
        effect->RandomizeParams();
      }

      effect->ChooseLights();
      effect->ReloadParams();
    }

    effect->Run();

    if (millis() > autoEffectAt) {
      effectIndex = random(effects.size());
      autoEffectAt =
          millis() + kAutoEffectBaseMs + random(kAutoEffectRandomMs);
      autoEffectTriggered = true;

      // Skip the delay to reduce the chance of flickering
      continue;
    }

    delay(10);
  }
}
