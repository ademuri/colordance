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

  HSV hsv = {HUE_RED, 255, 127};

  // Every so often, flash some lights to entice people to come play
  unsigned long enticeAtMs = 0;
  bool entice = false;
  unsigned long enticeEndAtMs = 0;
  // const unsigned long kEnticeEveryMs = 2 * 60 * 1000;
  // const unsigned long kEnticeRandom = 60 * 1000;
  const unsigned long kEnticeEveryMs = 5 * 1000;
  const unsigned long kEnticeEveryRandom = 5 * 1000;
  const unsigned long kEnticeDurationMs = 5 * 1000;
  const unsigned long kEnticeDurationRandom = 5 * 1000;
};

#endif
