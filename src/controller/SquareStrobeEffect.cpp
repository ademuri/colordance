#include "SquareStrobeEffect.hpp"
#include <cmath>
#include "LightController.hpp"

SquareStrobeEffect::SquareStrobeEffect(LightController *lightController,
                                       ParamController *paramController)
    : Effect(lightController, paramController) {
  ChooseLights();
}

void SquareStrobeEffect::BeatDetected() {}

void SquareStrobeEffect::SetLights() {
  for (unsigned int i = 0; i < aLights.size(); i++) {
    hues[i].v = (1 + sin(time * 3.14)) * 127.5;
    lightController->Set(lightIds[aLights[i]], hues[i]);
  }
  for (unsigned int i = 0; i < bLights.size(); i++) {
    // Offset the brightness by 90 degrees
    hues[aLights.size() + i].v = (1 + sin(time * 3.14 + 3.14)) * 127.5;
    lightController->Set(lightIds[bLights[i]], hues[aLights.size() + i]);
  }
}

void SquareStrobeEffect::DoRun() {
  // TODO: upgrade this to tap tempo or at least backoff so that the user can
  // tap slower than tempo
  if (paramController->Boost()) {
    tempo = tempoFromParams / 2;
  } else {
    tempo = tempoFromParams;
  }
  SetLights();

  time += 5.0 / tempo;
  SleepMs(5);
}

void SquareStrobeEffect::ParamChanged(Params param) {
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
      hues[0].s = hues[1].s = hues[2].s = hues[3].s =
          paramController->GetScaled(Params::kParam1, 0, 255);
      break;

    case Params::kParam2:
      break;

    case Params::kTempo:
      // Get a tempo ~in the range (1000, 200) ms.
      tempoFromParams = paramController->GetScaled(Params::kTempo, 700, 520);
      tempoFromParams = exp(tempoFromParams / 100.0);
      break;

    case Params::kOrientation:
      switch (paramController->GetScaled(Params::kOrientation, 0, 3)) {
        case 0:
        default:
          aLights = {0, 1};
          bLights = {2, 3};
          break;

        case 1:
          aLights = {1, 2};
          bLights = {3, 0};
          break;

        case 2:
          aLights = {0, 2};
          bLights = {1, 3};
          break;

        case 3:
          aLights = {3};
          bLights = {0, 1, 2};
          break;
      }

    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
      break;
  }
}

void SquareStrobeEffect::ChooseLights() {
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

  ParamChanged(Params::kOrientation);
}

void SquareStrobeEffect::RandomizeParams() {
#ifdef ARDUINO
  paramController->SetScaled(Params::kTempo, random(100), 0, 100);
  paramController->SetScaled(Params::kWidth, random(5), 0, 4);
  paramController->SetScaled(Params::kPan, random(5), 0, 4);
  paramController->SetScaled(Params::kTilt, random(5), 0, 4);
  // Height
  paramController->SetScaled(Params::kParam2, random(5), 0, 4);
  // Saturation
  paramController->SetScaled(Params::kParam1, 100 + random(155), 0, 255);

  paramController->SetScaled(Params::kHue0, random(360), 0, 359);
  paramController->SetScaled(Params::kHue1, random(360), 0, 359);
  paramController->SetScaled(Params::kHue2, random(360), 0, 359);
  paramController->SetScaled(Params::kKnob, random(360), 0, 359);
#endif
}
