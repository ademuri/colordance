#include "ChromaticAberrationEffect.hpp"

ChromaticAberrationEffect::ChromaticAberrationEffect(
    LightController *lightController, ParamController *paramController)
    : Effect(lightController, paramController) {
      ChooseLights();
    }

void ChromaticAberrationEffect::ChooseLights() {
  std::vector<uint16_t> oldLightIds = lightIds;
  lightIds = lightController->GetLights(paramController, 1, lightController->numCols)[0];
  TurnOffUnusedLights(oldLightIds, lightIds);
}

void ChromaticAberrationEffect::ParamChanged(Params param) {
  switch (param) {
    case Params::kParam:
      w = paramController->GetScaled(Params::kParam, 0, 100) / 100.0;
      break;

    // TODO: handle other cases
    case Params::kTempo:
    case Params::kHue0:
    case Params::kHue1:
    case Params::kHue2:
    case Params::kWidth:
    case Params::kPan:
    case Params::kTilt:
      break;
  }
}

void ChromaticAberrationEffect::DoRun() {
  int numLights = lightIds.size();
  printf("numLights: %d\n", numLights);
  printf("w: %f\n", w);
  float middleLight = (numLights - 1) / 2.0;
  for (int i = 0; i < numLights; i++) {
    // float pos = c + (i - middlelight) * w;
    float pos = (((float) i / (numLights - 0)) - c);
    HSV hsv = {getHue(pos), getSaturation(pos), 100};
    lightController->Set(lightIds[i], hsv);
    printf("i: %d, pos: %f, h: %3d, s: %3d\n", i, pos, hsv.h, hsv.s);
  }
  printf("\n");
}

uint16_t ChromaticAberrationEffect::getHue(float pos) {
  // Map to hue-space
  return pos * w * 360;
}

uint16_t ChromaticAberrationEffect::getSaturation(float pos) {
  pos /= w;
  // Map to saturation-space
  if (pos < 0) {
    pos *= -1;
  }

  const float saturationBandWidth = 1;
  if (pos < 0.5) {
    return 255;
  } else if(pos < 0.5 + saturationBandWidth) {
    return 255 - (pos - 0.5) / saturationBandWidth * 255;
  } else {
    return 0;
  }
}
