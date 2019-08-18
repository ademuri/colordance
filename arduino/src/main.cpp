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
#include "DirectLightController.hpp"
#include "DirectParamController.hpp"
#include "WProgram.h"

template <typename... Args>
void log(const std::string &format, Args... args) {
  static char buffer[kLogBufferSize];
  // Print the time and then the caller's message
  snprintf(buffer, kLogBufferSize, "%lu,", now());
  Serial5.print(buffer);

  snprintf(buffer, kLogBufferSize, format.c_str(), args...);
  Serial5.println(buffer);
}

extern "C" int main(void) {
  pinMode(13, OUTPUT);
  Serial5.begin(500000);

  pinMode(kScreenMotionPin, INPUT);
  pinMode(kControlMotionPin, INPUT);

  // Tells the Teensy to use the onboard RTC
  setSyncProvider((long int (*)())Teensy3Clock.get);

  // Log the header
  Serial5.println(
      "time,census,effectIndex,sleeping,usingAutoEffect,screenMotion,"
      "controlMotion,controlUsed");

  DirectLightController *lightController = new DirectLightController();
  DirectParamController *paramController = new DirectParamController();

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
  paramController->setNumEffects(effects.size());

  SleepEffect *sleepEffect = new SleepEffect(lightController, paramController);

  // Clear any accidentally on lights from the constructors
  lightController->Blackout();
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
    const uint8_t serialLedBrightness = 255;
    uint16_t currentHue = (millis() / 100) % 360;
    for (unsigned int i = 0; i < lightController->leds.size() - 6; i++) {
      HSV hsv = {currentHue, 255, serialLedBrightness};
      lightController->leds[i] = hsv;
    }
    // lightController->leds[0] = HSV{sleeping ? 0 : 120, 255, 255};

    // TODO: test this
    if (flashingControls) {
      if (millis() > flashControlsEndAt) {
        flashingControls = false;
        flashControlsAt = millis() + 2 * flashControlsEvery;
        lightController->SetButtonLights(false);
      }
      for (int i = 44; i <= 49; i++) {
        const bool lightsOn = (millis() / 100) % 2 == 0;
        const uint8_t brightness = lightsOn ? 0 : serialLedBrightness;
        lightController->leds[i] = HSV{HUE_GREEN, 255, brightness};
        lightController->SetButtonLights(lightsOn);
      }
    } else if (millis() > flashControlsAt) {
      flashingControls = true;
      flashControlsEndAt = millis() + flashControlsDuration;
    } else {
      lightController->SetButtonLights(false);
      lightController->leds[44] = lightController->leds[47] =
          HSV{currentHue, 255, serialLedBrightness};
      lightController->leds[45] = lightController->leds[48] =
          HSV{currentHue + 120, 255, serialLedBrightness};
      lightController->leds[46] = lightController->leds[49] =
          HSV{currentHue + 240, 255, serialLedBrightness};
      // Turn off one of the box lights to indicate which effect is active
      // Note: there should only be 6 effects, so the mod is for safety.
      lightController->leds[44 + (effectIndex % 6)] = HSV{0, 0, 0};
    }

    bool motion = false;
    if (analogRead(kScreenMotionPin) > kMotionThresh) {
      motion = true;
      screenMotionAt = millis();
    }
    if (analogRead(kControlMotionPin) > kMotionThresh) {
      motion = true;
      controlMotionAt = millis();
    }
    if (motion) {
      sleepAt = millis() + kNoInteractionSleepMs;
    }

    if (sleeping) {
      sleepEffect->Run();
      if (motion ||
          paramController->ScanForChanges(effect) != ParamChanged::kNone) {
        sleeping = false;
        sleepAt = millis() + kNoInteractionSleepMs;
        lightController->Blackout();
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
        controlUsedAt = millis();
        usingAutoEffect = false;
        flashControlsAt = millis() + flashControlsEvery;
      }

      if (millis() > sleepAt) {
        sleeping = true;
        lightController->Blackout();
        sleepEffect->ChooseLights();
        continue;
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

      // end of awake
    }

    // Census: log certain values every so often. Log CSV-like as "name, value"
    if (millis() > censusLogAt) {
      // Note: this seems to take about 1-2ms.
      bool screenMotion = screenMotionAt > lastCensusAt;
      bool controlMotion = controlMotionAt > lastCensusAt;
      bool controlUsed = controlUsedAt > lastCensusAt;
      // NOTE: keep this in sync with the log statement at the beginning of main
      log("census,%d,%d,%d,%d,%d,%d", effectIndex, sleeping, usingAutoEffect,
          screenMotion, controlMotion, controlUsed);

      lastCensusAt = millis();
      censusLogAt = millis() + kCensusLogMs;
    }

    delay(1);
  }
}
