#ifndef __EFFECT_HPP__
#define __EFFECT_HPP__

class Effect {
public:
  Effect() {}

  /* Called periodically to perform the effect. */
  virtual void run() = 0;
}

#endif
