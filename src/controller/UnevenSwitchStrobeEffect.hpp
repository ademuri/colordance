#ifndef __UNEVEN_SWITCH_STROBE_EFFECT_HPP__
#define __UNEVEN_SWITCH_STROBE_EFFECT_HPP__

#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

class UnevenSwitchStrobeEffect : public Effect {
 public:
  UnevenSwitchStrobeEffect(LightController *lightController,
                           ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  HSV hsv1 = {HUE_RED, 255, 255};
  HSV hsv2 = {HUE_BLUE, 255, 255};
  int16_t hsvShift = 0;
  int16_t hsvAdvance = 1;
  // Counter for where we are in the on-off cycle
  int16_t strobeCounter = 0;
};

#endif
