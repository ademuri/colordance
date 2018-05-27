#include "SquareEffect.hpp"
#include "LightController.hpp"

SquareEffect::SquareEffect(LightController *lightController,
                           ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SquareEffect::BeatDetected() {}

void SquareEffect::DoRun() {
  shiftPosition += tempo;

  for (unsigned int i = 0; i < kNumLights; i++) {
    const int32_t hueIndex = (i + shiftPosition / kStepsPerHue) % kNumLights;
    const int32_t hueOffset = shiftPosition % kStepsPerHue;

    actualHues[i] = Color::interpolate(baseHues[hueIndex],
                                       baseHues[(hueIndex + 1) % kNumLights],
                                       hueOffset * 255 / kStepsPerHue);

    lightController->Set(lightIds[i], actualHues[i]);
  }
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
      baseHues[0].v = baseHues[1].v = baseHues[2].v =
          paramController->GetScaled(Params::kParam1, 0, 255);
      break;

    case Params::kParam2:
      baseHues[0].s = baseHues[1].s = baseHues[2].s =
          paramController->GetScaled(Params::kParam2, 0, 255);
      break;

    case Params::kTempo:
      tempo = paramController->GetScaled(Params::kTempo, 0, 5);
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

  tempo = 1;
}
