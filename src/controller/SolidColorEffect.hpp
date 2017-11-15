#ifndef __SOLID_COLOR_EFFECT_HPP__
#define __SOLID_COLOR_EFFECT_HPP__

#include "Effect.hpp"
#include "LightController.hpp"

class SolidColorEffect : public Effect {
public:
  SolidColorEffect(LightController *controller);

  /* Called periodically to perform the effect. */
  void run() override;
};

#endif
