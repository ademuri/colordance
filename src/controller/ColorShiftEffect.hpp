#ifndef __COLOR_SHIFT_EFFECT_HPP__
#define __COLOR_SHIFT_EFFECT_HPP__

#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

enum class ColorShiftMode { straight, ring };

class ColorShiftEffect : public Effect {
 public:
  ColorShiftEffect(LightController *lightController,
                   ParamController *paramController, ColorShiftMode mode);

  void BeatDetected() override;
  void ChooseLights() override;
  void ParamChanged(Params param) override;
  void RandomizeParams() override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  // This effect has multiple "modes". Each moe has a different method of
  // choosing lights. Once the lights are chosen, the behavior is the same.
  void ChooseLightsStraight();
  void ChooseLightsRing();

  std::vector<int16_t> lightIds;

  HSV hsv = {C_HUE_RED, 255, 255};
  int16_t hsvShift = 0;
  int16_t hsvAdvance = 1;

  const int16_t kMaxLights = 5;

  const ColorShiftMode mode;
};

#endif
