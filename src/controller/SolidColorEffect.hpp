#ifndef __SOLID_COLOR_EFFECT_HPP__
#define __SOLID_COLOR_EFFECT_HPP__

#include <algorithm>
#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

class SolidColorEffect : public Effect {
 public:
  SolidColorEffect(LightController *lightController,
                   ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  HSV hsv = {HUE_RED, 255, 255};
  int16_t hsvShift = 0;
};

#endif
