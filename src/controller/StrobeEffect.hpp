#ifndef __STROBE_EFFECT_HPP__
#define __STROBE_EFFECT_HPP__

#include "Effect.hpp"
#include "LightController.hpp"

class StrobeEffect : public Effect {
public:
  StrobeEffect(LightController *lightController,
               ParamController *paramController);

  void BeatDetected() override;

protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

private:
  bool on = false;

  HSV hsv1 = {HUE_RED, 255, 255};
  HSV hsv2 = {HUE_GREEN, 255, 255};
};

#endif
