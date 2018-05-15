// Note: this needs to be before the <cmath> include, because Arduino includes
// an abs macro that clobbers the std::abs. Putting a break after this import
// prevents clang-format from reordering it.
#ifdef ARDUINO
#include "WProgram.h"
#endif

#include "CircleStrobeEffect.hpp"
#include "LightController.hpp"

CircleStrobeEffect::CircleStrobeEffect(LightController *lightController,
                                       ParamController *paramController)
    : Effect(lightController, paramController) {
  hsv.h = paramController->Get(Params::kHue0);
  ChooseLights();
}

void CircleStrobeEffect::setIndex(int16_t index, const HSV &hsv) {
  HSV adjustedHsv = hsv;
  adjustedHsv.h += hueAdjust;
  lightController->Set(lightIds[index], adjustedHsv);

  HSV secondHsv = adjustedHsv;
  secondHsv.h = adjustedHsv.h + 20;
  lightController->Set(lightIds[(index + 1) % lightIds.size()], secondHsv);

  if (index == 0) {
    if (lightIds.size() > 1) {
      lightController->Set(lightIds[lightIds.size() - 1], {0, 0, 0});
    }
  } else {
    lightController->Set(lightIds[index - 1], {0, 0, 0});
  }
}

void CircleStrobeEffect::DoRun() {
  currentLight++;
  if (currentLight >= lightIds.size()) {
    currentLight = 0;
  }

  setIndex(currentLight, hsv);

  HSV second = hsv;
  second.h = hsv.h + 180;
  setIndex((currentLight + lightIds.size() / 2) % lightIds.size(), second);

  hsv.h += 2;
  SleepMs(paramController->GetScaled(Params::kTempo, 500, 50));
}

void CircleStrobeEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;
  lightIds.clear();

  std::vector<std::vector<int16_t>> lightArray = lightController->GetLights(
      paramController,
      paramController->GetScaled(Params::kParam2, 2, lightController->numRows),
      paramController->GetScaled(Params::kWidth, 2, lightController->numCols));
  const int colSize = lightArray[0].size();

  // Grab the top row
  for (int i = 1; i < colSize - 1; i++) {
    lightIds.push_back(lightArray[0][i]);
  }

  for (int i = 1; i < lightArray.size() - 1; i++) {
    lightIds.push_back(lightArray[i][colSize - 1]);
  }

  for (int i = colSize - 2; i >= 1; i--) {
    lightIds.push_back(lightArray[lightArray.size() - 1][i]);
  }

  for (int i = lightArray.size() - 2; i >= 1; i--) {
    lightIds.push_back(lightArray[i][0]);
  }

  hueAdjust = 320 / lightIds.size();
  TurnOffUnusedLights(oldLightIds, lightIds);
}

void CircleStrobeEffect::BeatDetected() {
  hsv.h += 30;
  DoRun();
}

void CircleStrobeEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(70), 0, 100);
  paramController->SetScaled(Params::kWidth, 2 + random(3), 0, 4);
  paramController->SetScaled(Params::kParam2, 2 + random(3), 0, 4);
  paramController->SetScaled(Params::kPan, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
#endif
}
