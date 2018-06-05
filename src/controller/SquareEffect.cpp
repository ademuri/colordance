#include "SquareEffect.hpp"
#include "LightController.hpp"

SquareEffect::SquareEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SquareEffect::BeatDetected() {}

void SquareEffect::DoRun() {
  if (tempo != 0) {
    hueOsc += hueDirection;
  }

  if (hueOsc > hueBandWidth) {
    hueDirection = -1;
  } else if (hueOsc <= 0) {
    hueDirection = 1;
  }

  for (unsigned int i = 0; i < kNumLights; i++) {
    actualHues[i].h = baseHues[i].h + hueOsc;
    lightController->Set(lightIds[i], actualHues[i]);
  }

  SleepMs((kMaxTempo - tempo));
}

void SquareEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kHue0:
      baseHues[0].h = paramController->GetScaled(Params::kHue0, 0, 359);
      break;

    case Params::kHue1:
      baseHues[1].h = paramController->GetScaled(Params::kHue1, 0, 359);
      break;

    case Params::kHue2:
      baseHues[2].h = paramController->GetScaled(Params::kHue2, 0, 359);
      break;

    case Params::kKnob:
      baseHues[3].h = paramController->GetScaled(Params::kKnob, 0, 359);
      break;

    case Params::kParam1:
      hueBandWidth = paramController->GetScaled(Params::kParam1, 5, 60);
      break;

    case Params::kParam2:
      break;

    case Params::kTempo:
      tempo = paramController->GetScaled(Params::kTempo, 0, kMaxTempo);
      break;

    case Params::kWidth:
    case Params::kPan:
      break;
  }
}

void SquareEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;

  std::vector<std::vector<int16_t>> newLightIds = lightController->GetLights(
      paramController,
      paramController->GetScaled(Params::kParam2, 2, lightController->numRows),
      paramController->GetScaled(Params::kWidth, 2, lightController->numCols));
  lightIds.clear();
  lightIds.push_back(newLightIds[0][0]);
  lightIds.push_back(newLightIds[0][newLightIds[0].size() - 1]);
  lightIds.push_back(
      newLightIds[newLightIds.size() - 1][newLightIds[0].size() - 1]);
  lightIds.push_back(newLightIds[newLightIds.size() - 1][0]);
  TurnOffUnusedLights(oldLightIds, lightIds);
}

void SquareEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(90), 0, 100);
  paramController->SetScaled(Params::kWidth, random(5), 0, 4);
  paramController->SetScaled(Params::kPan, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
  // Height
  paramController->SetScaled(Params::kParam2, random(5), 0, 4);
  // Color bandwidth
  paramController->SetScaled(Params::kParam1, 100 + random(156), 100, 255);

  paramController->SetScaled(Params::kHue0, random(360), 0, 359);
  paramController->SetScaled(Params::kHue1, random(360), 0, 359);
  paramController->SetScaled(Params::kHue2, random(360), 0, 359);
  paramController->SetScaled(Params::kKnob, random(360), 0, 359);
#endif
}
