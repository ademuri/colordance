#ifndef __RING_EFFECT_HPP__
#define __RING_EFFECT_HPP__

#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

class RingEffect : public Effect {
 public:
  RingEffect(LightController *lightController,
             ParamController *paramController);

  void BeatDetected() override;
  void ChooseLights() override;
  void ParamChanged(Params param) override;
  void RandomizeParams() override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  // Note: this isn't directly used. It just contains the ids from the lightsN
  // vectors for cleanup purposes.
  std::vector<int16_t> lightIds;

  std::vector<int16_t> lights0;
  std::vector<int16_t> lights1;
  std::vector<int16_t> lights2;

  HSV hsv = {C_HUE_RED, 255, 255};
  int16_t hsvShift = 0;
  int16_t hsvAdvance = 1;
};

#endif
