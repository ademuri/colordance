#ifndef __LIGHT_CONTROLLER_HPP__
#define __LIGHT_CONTROLLER_HPP__

#include "../color/Color.hpp"

/**
 * References to the individual, physical lights.
 */
enum class Lights {
  STAGE_LEFT,
  CENTER,
  STAGE_RIGHT,
  TOP,
  BOTTOM,
};

/*
 * Interface class for controlling the output lights. Eventually, there will be
 * a subclass of this that uses DMX to control real lights.
 */
class LightController {
public:
  LightController() {}

  virtual void Set(const Lights light, HSV hsv) = 0;
};

#endif
