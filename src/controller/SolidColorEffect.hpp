#ifndef __SOLID_COLOR_EFFECT_HPP__
#define __SOLID_COLOR_EFFECT_HPP__

#include "Effect.hpp"

class SolidColorEffect : public Effect {
public:
  SolidColorEffect();

  /* Called periodically to perform the effect. */
  void run() override;
};

#endif

