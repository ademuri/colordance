#ifndef __SQUARE_EFFECT_HPP__
#define __SQUARE_EFFECT_HPP__

#include <algorithm>
#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

/*
 * Simple effect that has four lights in a square. The color of each
 * light is controllable by the user. The lights are chosen using the
 * width, pan, and tilt params.
 */
class SquareEffect : public Effect {
 public:
  SquareEffect(LightController *lightController,
               ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  std::vector<HSV> hues = {
      {0, 255, 255}, {90, 255, 255}, {180, 255, 255}, {270, 255, 255},
  };

  const unsigned int kNumLights = 4;
};

#endif
