#ifndef __THREE_COLOR_EFFECT_HPP__
#define __THREE_COLOR_EFFECT_HPP__

#include <algorithm>
#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

/*
 * Simple effect that has three lights. The color of each light is controllable
 * by the user. The lights are chosen using the width, pan, and tilt params.
 */
class ThreeColorEffect : public Effect {
 public:
  ThreeColorEffect(LightController *lightController,
                   ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  uint16_t light1;
  uint16_t light2;
  uint16_t light3;

  HSV hsv1 = {HUE_RED, 255, 200};
  HSV hsv2 = {HUE_GREEN, 255, 200};
  HSV hsv3 = {HUE_BLUE, 255, 200};
};

#endif
