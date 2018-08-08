#include "RingEffect.hpp"
#include <cmath>
#include "LightController.hpp"

RingEffect::RingEffect(LightController *lightController,
                       ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
}

void RingEffect::DoRun() {
  HSV toSet = hsv;
  if (paramController->Boost()) {
    // Swap the inside and outside colors (ish)
    toSet.h -= hsvShift;
  }
  toSet.v = 255 / lights0.size();
  for (auto id : lights0) {
    lightController->Set(id, toSet);
  }

  toSet.h += hsvShift;
  toSet.v = 255 / lights1.size();
  for (auto id : lights1) {
    lightController->Set(id, toSet);
  }

  toSet.h += hsvShift;
  toSet.v = 255 / lights2.size();
  for (auto id : lights2) {
    lightController->Set(id, toSet);
  }

  hsv.h += hsvAdvance;
  SleepMs(5);
}

void RingEffect::BeatDetected() { hsv.h += 60; }

void RingEffect::ParamChanged(Params param) {
  // TODO: wire up params
  switch (param) {
    case Params::kTempo:
      hsvAdvance = paramController->GetScaled(Params::kTempo, 1, 8);
      break;

    case Params::kParam2:
      hsv.s = paramController->GetScaled(Params::kParam2, 100, 255);
      break;

    case Params::kParam1:
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

void RingEffect::ChooseLights() {
  std::vector<int16_t> oldLightIds = lightIds;
  std::vector<std::vector<int16_t>> lightArray =
      lightController->GetLights(paramController, 3, 3);

  // TODO: more patterns
  lights0.push_back(lightArray[1][1]);
  lights1.push_back(lightArray[1][0]);
  lights1.push_back(lightArray[1][2]);
  lights2.push_back(lightArray[0][0]);
  lights2.push_back(lightArray[0][2]);
  lights2.push_back(lightArray[2][0]);
  lights2.push_back(lightArray[2][2]);

  lightIds.insert(lightIds.end(), lights0.begin(), lights0.end());
  lightIds.insert(lightIds.end(), lights1.begin(), lights1.end());
  lightIds.insert(lightIds.end(), lights2.begin(), lights2.end());

  hsvShift = 360 / 3;

  TurnOffUnusedLights(oldLightIds, lightIds);
}

void RingEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(60), 0, 100);
  paramController->SetScaled(Params::kWidth, 1 + random(4), 0, 4);
  paramController->SetScaled(Params::kPan, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
  paramController->SetScaled(Params::kParam1, 100 + random(156), 100, 255);
  paramController->SetScaled(Params::kParam2, 100 + random(156), 100, 255);
#endif
}
