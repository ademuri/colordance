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
  /** Utility to set the given light (and maybe adjacent lights) */
  void setIndex(int16_t index, const HSV &hsv);

  std::vector<int16_t> lightIds;

  int16_t currentLight = 0;
  int16_t hueAdjust = 0;

  HSV hsv = {HUE_RED, 255, 255};
};

#endif
