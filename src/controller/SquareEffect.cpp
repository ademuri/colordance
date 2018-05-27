#include "SquareEffect.hpp"
#include "LightController.hpp"

SquareEffect::SquareEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SquareEffect::BeatDetected() {}

void SquareEffect::DoRun() {
  for (unsigned int i = 0; i < kNumLights; i++) {
    lightController->Set(lightIds[i], hues[i]);
  }
}

void SquareEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kHue0:
      hues[0].h = paramController->GetScaled(Params::kHue0, 0, 359);
      break;

    case Params::kHue1:
      hues[1].h = paramController->GetScaled(Params::kHue1, 0, 359);
      break;

    case Params::kHue2:
      hues[2].h = paramController->GetScaled(Params::kHue2, 0, 359);
      break;

    case Params::kKnob:
      hues[3].h = paramController->GetScaled(Params::kKnob, 0, 359);
      break;

    case Params::kParam1:
      hues[0].v = hues[1].v = hues[2].v =
          paramController->GetScaled(Params::kParam1, 0, 255);
      break;

    case Params::kParam2:
      hues[0].s = hues[1].s = hues[2].s =
          paramController->GetScaled(Params::kParam2, 0, 255);
      break;

    // TODO: handle other cases
    case Params::kTempo:
    case Params::kWidth:
    case Params::kPan:
      break;
  }
}

void SquareEffect::ChooseLights() {
  // Keep track of the lights that were on before, and turn them off if they're
  // no longer selected.
  std::vector<int16_t> oldLightIds = lightIds;

  unsigned int size =
      paramController->GetScaled(Params::kWidth, 2, lightController->numRows);
  std::vector<std::vector<int16_t>> newLightIds =
      lightController->GetLights(paramController, size, size);
  lightIds.clear();
  lightIds.push_back(newLightIds[0][0]);
  lightIds.push_back(newLightIds[0][newLightIds[0].size() - 1]);
  lightIds.push_back(
      newLightIds[newLightIds.size() - 1][newLightIds[0].size() - 1]);
  lightIds.push_back(newLightIds[newLightIds.size() - 1][0]);
  TurnOffUnusedLights(oldLightIds, lightIds);
}
