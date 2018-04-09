#ifndef __CIRCLE_STROBE_EFFECT_HPP__
#define __CIRCLE_STROBE_EFFECT_HPP__

#include "Effect.hpp"
#include "LightController.hpp"

class CircleStrobeEffect : public Effect {
 public:
  CircleStrobeEffect(LightController *lightController,
                     ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<uint16_t> lightIds;

  uint16_t currentLight = 0;
  uint16_t hueAdjust = 0;

  HSV hsv = {HUE_RED, 255, 255};
};

#endif
