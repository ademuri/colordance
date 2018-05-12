#ifndef __STROBE_EFFECT_HPP__
#define __STROBE_EFFECT_HPP__

#include "Effect.hpp"
#include "LightController.hpp"

class StrobeEffect : public Effect {
 public:
  StrobeEffect(LightController *lightController,
               ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  int16_t currentLight = 0;
  int16_t hueAdjust = 0;

  HSV hsv = {HUE_RED, 255, 255};
};

#endif
