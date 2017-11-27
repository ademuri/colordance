#ifndef __SOLID_COLOR_EFFECT_HPP__
#define __SOLID_COLOR_EFFECT_HPP__

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
  std::vector<uint16_t> lightIds;

  HSV hsv = {HUE_RED, 255, 255};
  uint16_t hsvShift = 0;
};

#endif
