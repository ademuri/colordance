#include "UnevenSwitchStrobeEffect.hpp"
#include "LightController.hpp"

UnevenSwitchStrobeEffect::UnevenSwitchStrobeEffect(
    LightController *lightController, ParamController *paramController)
    : Effect(lightController, paramController) {
  // Choose lights
  ChooseLights();
  hsvAdvance = 0;
}

void UnevenSwitchStrobeEffect::DoRun() {
  HSV off = {0, 0, 0};

  switch (strobeCounter) {
    case 0:
      lightController->Set(lightIds[0], hsv1);
      lightController->Set(lightIds[1], hsv2);
      SleepMs(100);
      break;

    case 1:
      lightController->Set(lightIds[0], off);
      lightController->Set(lightIds[1], off);
      SleepMs(100);
      break;

    case 2:
      lightController->Set(lightIds[0], hsv2);
      lightController->Set(lightIds[1], hsv1);
      SleepMs(100);
      break;

    case 3:
      lightController->Set(lightIds[0], off);
      lightController->Set(lightIds[1], off);
      SleepMs(paramController->GetScaled(Params::kTempo, 1000, 200));
      break;

    default:
      // TODO: error handling
      strobeCounter = 0;
  }

  strobeCounter++;
  if (strobeCounter > 3) {
    strobeCounter = 0;
  }
}

void UnevenSwitchStrobeEffect::BeatDetected() {}

void UnevenSwitchStrobeEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kTempo:
      // hsvAdvance = paramController->GetScaled(Params::kTempo, 0, 10);
      hsvAdvance = 0;
      break;

    // TODO: handle other cases
    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
      break;
  }
}

void UnevenSwitchStrobeEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<uint16_t> oldLightIds = lightIds;

  lightIds = lightController->GetLights(paramController, 1, 2)[0];

  TurnOffUnusedLights(oldLightIds, lightIds);
}
