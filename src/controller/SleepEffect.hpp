#ifndef __SLEEP_EFFECT_HPP__
#define __SLEEP_EFFECT_HPP__

#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

class SleepEffect : public Effect {
 public:
  SleepEffect(LightController *lightController,
              ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  // TODO: tune the brightness
  HSV hsv = {HUE_RED, 255, 127};
};

#endif
