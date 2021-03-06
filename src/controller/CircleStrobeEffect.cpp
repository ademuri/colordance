#include "CircleStrobeEffect.hpp"
#include "LightController.hpp"

#include <cmath>

CircleStrobeEffect::CircleStrobeEffect(LightController *lightController,
                                       ParamController *paramController)
    : Effect(lightController, paramController) {
  hsv.h = paramController->Get(Params::kHue0);
  ChooseLights();
}

void CircleStrobeEffect::setIndex(int16_t index, const HSV &hsv) {
  lightController->Set(lightIds[index], hsv);

  HSV secondHsv = hsv;
  secondHsv.h = hsv.h + hueGap;
  lightController->Set(lightIds[(index + 1) % lightIds.size()], secondHsv);

  if (lightIds.size() > 4) {
    if (index == 0) {
      if (lightIds.size() > 1) {
        lightController->Set(lightIds[lightIds.size() - 1], {0, 0, 0});
      }
    } else {
      lightController->Set(lightIds[index - 1], {0, 0, 0});
    }
  }
}

void CircleStrobeEffect::DoRun() {
  // TODO: consolidate this logic with square strobe
  if (paramController->Boost() && !prevBoost) {
    stepAt = 0;
  }
  prevBoost = paramController->Boost();

  if (lightController->GetMs() > stepAt) {
    currentLight++;
    if (currentLight >= (int16_t)lightIds.size()) {
      currentLight = 0;
    }
    SetLights();

    hsv.h += 1;
    // SleepMs(paramController->GetScaled(Params::kTempo, 500, 50));
    stepAt = lightController->GetMs() +
             paramController->GetScaled(Params::kTempo, 500, 50);
  }
}

void CircleStrobeEffect::SetLights() {
  HSV first = hsv;
  HSV second = hsv;
  second.h = hsv.h + hueDistance;

  setIndex(currentLight, first);
  setIndex((currentLight + lightIds.size() / 2) % lightIds.size(), second);
}

void CircleStrobeEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kHue0:
      hueGap = 5 + std::abs(paramController->GetScaled(Params::kHue0, -55, 55));
      break;

    case Params::kHue1:
      hueDistance =
          90 + std::abs(paramController->GetScaled(Params::kHue1, -90, 90));
      break;

    case Params::kParam1:
      hsv.s = paramController->GetScaled(Params::kParam1, 100, 255);
      break;

    case Params::kTempo:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
    case Params::kOrientation:
    case Params::kParam2:
    case Params::kKnob:
      break;
  }
}

void CircleStrobeEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;
  lightIds.clear();

  std::vector<std::vector<int16_t>> lightArray = lightController->GetLights(
      paramController,
      paramController->GetScaled(Params::kParam2, 3, lightController->numRows),
      paramController->GetScaled(Params::kWidth, 3, lightController->numCols));
  const int colSize = lightArray[0].size();

  // Grab the top row
  for (int i = 1; i < colSize - 1; i++) {
    lightIds.push_back(lightArray[0][i]);
  }

  for (int i = 1; i < (int16_t)lightArray.size() - 1; i++) {
    lightIds.push_back(lightArray[i][colSize - 1]);
  }

  for (int i = colSize - 2; i >= 1; i--) {
    lightIds.push_back(lightArray[lightArray.size() - 1][i]);
  }

  for (int i = lightArray.size() - 2; i >= 1; i--) {
    lightIds.push_back(lightArray[i][0]);
  }

  lightController->Blackout();
  SetLights();
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
  paramController->SetScaled(Params::kParam1, 100 + random(156), 0, 255);
  paramController->SetScaled(Params::kHue0, 0, random(40), 100);
  paramController->SetScaled(Params::kHue1, 0, random(40), 100);
#endif
}
