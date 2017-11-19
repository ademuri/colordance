#ifndef __EFFECT_HPP__
#define __EFFECT_HPP__

#include "LightController.hpp"

class Effect {
public:
  Effect(LightController *controller);

  /*
   * Called periodically to perform the effect. This is called by the main
   * loop. Subclasses implement DoRun, below. DoRun contains the
   * effect-specific code. This method contains glue code and calls DoRun. This
   * allows e.g. for an effect to ask to be run every certain number of
   * milliseconds.
   */
  void Run();

protected:
  /* Effect-specific code. Must be overriden by subclasses. */
  virtual void DoRun() = 0;

  /*
   * Sleeps this effect for the specified number of milliseconds. DoRun won't be
   * called until this elapses.
   */
  void SleepMs(uint16_t ms);

  LightController *const controller;

private:
  /* When to call DoRun again. Used for SleepMs. */
  uint16_t runAtMs = 0;
};

#endif
