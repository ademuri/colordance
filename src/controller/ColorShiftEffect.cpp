#include "ColorShiftEffect.hpp"
#include <cmath>
#include "LightController.hpp"

ColorShiftEffect::ColorShiftEffect(LightController *lightController,
                                   ParamController *paramController,
                                   ColorShiftMode mode)
    : Effect(lightController, paramController), mode(mode) {
  // Choose lights
  ChooseLights();
}

void ColorShiftEffect::DoRun() {
  int sign = paramController->Boost() ? -1 : 1;
  int step = 1;
  int offset = 0;
  if ((int16_t)lightIds.size() > kMaxLights) {
    step = lightIds.size() / kMaxLights;
    offset = paramController->GetScaled(Params::kKnob, 0,
                                        lightIds.size() - (step * kMaxLights));
  }

  // The light ids that were used
  std::vector<int> usedLights;
  for (int16_t i = 0; i < kMaxLights && i < lightIds.size(); i++) {
    int lightIndex = i * step + offset;
    lightController->Set(
        lightIds[lightIndex],
        {(uint16_t)(hsv.h + sign * i * hsvShift), hsv.s, hsv.v});
    usedLights.push_back(lightIds[lightIndex]);
  }

  // Turn off the lights that weren't used.
  for (int i = 0; i < lightIds.size(); i++) {
    if (std::find(usedLights.begin(), usedLights.end(), lightIds[i]) ==
        usedLights.end()) {
      lightController->Set(lightIds[i], {0, 0, 0});
    }
  }

  hsv.h += hsvAdvance;
  SleepMs(5);
}

void ColorShiftEffect::BeatDetected() { hsv.h += 60; }

void ColorShiftEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kTempo:
      hsvAdvance = paramController->GetScaled(Params::kTempo, 1, 8);
      break;

    case Params::kParam1:
      hsvShift = paramController->GetScaled(Params::kParam1, 30,
                                            360 / lightIds.size());
      break;

    case Params::kParam2:
      hsv.s = paramController->GetScaled(Params::kParam2, 100, 255);
      break;

    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
    case Params::kOrientation:
    case Params::kKnob:
      break;
  }
}

void ColorShiftEffect::ChooseLightsStraight() {
  lightIds = lightController->GetLightsFromParams(paramController);
}

void ColorShiftEffect::ChooseLightsRing() {
  lightIds.clear();

  std::vector<std::vector<int16_t>> lightArray = lightController->GetLights(
      paramController,
      paramController->GetScaled(Params::kWidth, 2, lightController->numRows),
      paramController->GetScaled(Params::kWidth, 2, lightController->numCols));
  const int colSize = lightArray[0].size();
  // Grab the outside ring of lights
  for (int i = 0; i < colSize; i++) {
    lightIds.push_back(lightArray[0][i]);
  }

  for (int i = 1; i < (int16_t)lightArray.size() - 1; i++) {
    lightIds.push_back(lightArray[i][colSize - 1]);
  }

  for (int i = colSize - 1; i >= 0; i--) {
    lightIds.push_back(lightArray[lightArray.size() - 1][i]);
  }

  for (int i = lightArray.size() - 2; i >= 1; i--) {
    lightIds.push_back(lightArray[i][0]);
  }
}

void ColorShiftEffect::ChooseLights() {
  std::vector<int16_t> oldLightIds = lightIds;
  switch (mode) {
    case ColorShiftMode::straight:
      ChooseLightsStraight();
      break;

    case ColorShiftMode::ring:
      ChooseLightsRing();
      break;
  }
  hsvShift = paramController->GetScaled(
      Params::kParam1, 30,
      360 / std::min((int16_t)lightIds.size(), kMaxLights));

  TurnOffUnusedLights(oldLightIds, lightIds);
}

void ColorShiftEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(60), 0, 100);
  paramController->SetScaled(Params::kWidth, 1 + random(4), 0, 4);
  paramController->SetScaled(Params::kPan, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
  paramController->SetScaled(Params::kParam1, 100 + random(156), 100, 255);
  paramController->SetScaled(Params::kParam2, 100 + random(156), 100, 255);
#endif
}
