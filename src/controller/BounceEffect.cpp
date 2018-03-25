#include "BounceEffect.hpp"
#include "LightController.hpp"

BounceEffect::BounceEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
}

void BounceEffect::DoRun() {
  for (uint16_t i = 0; i < leftLight; i++) {
    lightController->Set(lightIds[i], {0, 0, 0});
  }
  for (uint16_t i = leftLight; i < leftLight + numLights; i++) {
    lightController->Set(lightIds[i], {hsv.h + i * hsvShift, hsv.s, hsv.v});
  }
  for (uint16_t i = leftLight + numLights; i < lightIds.size(); i++) {
    lightController->Set(lightIds[i], {0, 0, 0});
  }

  leftLight += step;
  if (leftLight + numLights > lightIds.size()) {
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

void BounceEffect::BeatDetected() { hsv.h += 60; }

void BounceEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kTempo:
      hsvAdvance = paramController->GetScaled(Params::kTempo, 1, 10);
      break;

    case Params::kWidth:
      numLights =
          paramController->GetScaled(Params::kWidth, 1, lightIds.size() - 1);
      if (leftLight + numLights > lightIds.size()) {
        leftLight = lightIds.size() - numLights;
      }
      break;

    // TODO: handle other cases
    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kPan:
    case Params::kTilt:
      break;
  }
}

void BounceEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<uint16_t> oldLightIds = lightIds;

  // lightIds = lightController->GetLights(paramController, 1,
  // paramController->GetScaled(Params::kWidth, 3, numCols));
  lightIds = lightController->GetLights(paramController, 1,
                                        lightController->numCols)[0];
  hsvShift = 360 / lightIds.size() + 20;
  hsv.v = 100 + (310 / lightIds.size());

  TurnOffUnusedLights(oldLightIds, lightIds);

  if (lightIds.size() > 1) {
    numLights =
        paramController->GetScaled(Params::kWidth, 1, lightIds.size() - 1);
    if (leftLight + numLights > lightIds.size()) {
      leftLight = lightIds.size() - numLights;
    }
  } else {
    numLights = 1;
    leftLight = 0;
  }
}
