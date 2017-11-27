#ifndef __COLOR_SHIFT_EFFECT_HPP__
#define __COLOR_SHIFT_EFFECT_HPP__

#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

class ColorShiftEffect : public Effect {
 public:
  ColorShiftEffect(LightController *lightController,
                   ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<uint16_t> lightIds;

  HSV hsv = {HUE_RED, 255, 255};
  uint16_t hsvShift = 0;
  uint16_t hsvAdvance = 1;
};

#endif