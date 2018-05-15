#ifndef __BOUNCE_EFFECT_HPP__
#define __BOUNCE_EFFECT_HPP__

#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

class BounceEffect : public Effect {
 public:
  BounceEffect(LightController *lightController,
               ParamController *paramController);

  void BeatDetected() override;
  void ChooseLights() override;
  void ParamChanged(Params param) override;
  void RandomizeParams() override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  int16_t numLights = 2;
  int16_t leftLight = 0;
  int16_t step = 1;

  HSV hsv = {HUE_RED, 255, 255};
  int16_t hsvShift = 0;
  int16_t hsvAdvance = 1;
};

#endif
