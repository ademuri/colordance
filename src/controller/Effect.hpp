#ifndef __EFFECT_HPP__
#define __EFFECT_HPP__

#include "LightController.hpp"

class Effect {
public:
  Effect(LightController *controller);

  /* Called periodically to perform the effect. */
  virtual void run() = 0;

protected:
  LightController *const controller;
};

#endif
