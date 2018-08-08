#include "BounceEffect.hpp"
#include "LightController.hpp"

BounceEffect::BounceEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
}

void BounceEffect::DoRun() {
  SetLights();

  leftLight += step;
  if (leftLight + numLights > (int16_t)lightIds.size()) {
    step = -1;
    leftLight--;
    hsv.h += hsvAdvance;
  } else if (leftLight < 0) {
    leftLight = 0;
    step = 1;
    hsv.h += hsvAdvance;
  }

  SleepMs(paramController->GetScaled(Params::kTempo, 200, 40));
}

void BounceEffect::SetLights() {
  if (paramController->Boost()) {
    hsv.s = 100;
  } else {
    hsv.s = 255;
  }
  for (int16_t i = 0; i < leftLight; i++) {
    lightController->Set(lightIds[i], {0, 0, 0});
  }
  for (int16_t i = leftLight; i < leftLight + numLights; i++) {
    lightController->Set(lightIds[i],
                         {(uint16_t)(hsv.h + i * hsvShift), hsv.s, hsv.v});
  }
  for (int16_t i = leftLight + numLights; i < (int16_t)lightIds.size(); i++) {
    lightController->Set(lightIds[i], {0, 0, 0});
  }
}

void BounceEffect::BeatDetected() { hsv.h += 60; }

void BounceEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kParam1:
      hsvAdvance = paramController->GetScaled(Params::kParam1, 2, 20);
      break;

    case Params::kWidth:
      numLights =
          paramController->GetScaled(Params::kWidth, 1, lightIds.size() - 1);
      if (leftLight + numLights > (int16_t)lightIds.size()) {
        leftLight = lightIds.size() - numLights;
      }
      break;

    case Params::kParam2:
      hsvShift =
          paramController->GetScaled(Params::kParam2, 30, 360 / numLights);
      break;

    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kPan:
    case Params::kTilt:
    case Params::kTempo:
    case Params::kOrientation:
    case Params::kKnob:
      break;
  }
}

void BounceEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLightsFromParams(
      paramController,
      std::max(lightController->numCols, lightController->numRows));

  TurnOffUnusedLights(oldLightIds, lightIds);

  ParamChanged(Params::kWidth);
}

void BounceEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(70), 0, 100);
  paramController->SetScaled(Params::kWidth, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
  paramController->SetScaled(Params::kParam1, random(256), 0, 255);
  paramController->SetScaled(Params::kParam2, random(256), 0, 255);
#endif
}
