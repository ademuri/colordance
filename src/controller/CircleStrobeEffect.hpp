#ifndef __CIRCLE_STROBE_EFFECT_HPP__
#define __CIRCLE_STROBE_EFFECT_HPP__

#include "Effect.hpp"
#include "LightController.hpp"

class CircleStrobeEffect : public Effect {
 public:
  CircleStrobeEffect(LightController *lightController,
                     ParamController *paramController);

  void BeatDetected() override;

  void ChooseLights() override;

  void ParamChanged(Params param) override;

  void RandomizeParams() override;

 protected:
  void DoRun() override;
  void SetLights() override;

 private:
  /** Utility to set the given light (and maybe adjacent lights) */
  void setIndex(int16_t index, const HSV &hsv);

  std::vector<int16_t> lightIds;

  int16_t currentLight = 0;
  // Hue distance between adjacent lights
  int16_t hueGap = 0;

  // Hue distance between the two groups
  int16_t hueDistance = 180;

  HSV hsv = {C_HUE_RED, 255, 255};

  /**
   * Previous state of the boost button. Used to trigger a strobe on the rising
   * edge.
   */
  bool prevBoost = false;

  unsigned long stepAt = 0;
};

#endif
