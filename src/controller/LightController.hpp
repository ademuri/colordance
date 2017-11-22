#ifndef __LIGHT_CONTROLLER_HPP__
#define __LIGHT_CONTROLLER_HPP__

#include "../color/Color.hpp"
#include "ParamController.hpp"
#include <vector>

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

  /*
   * Sets the specified light to an RGB value corresponding to the given HSV.
   */
  virtual void Set(const Lights light, HSV hsv) = 0;

  virtual std::vector<std::vector<uint8_t>>
  GetLights(ParamController *paramController, const int16_t rows,
            const int16_t cols);

  /*
   * Gets the number of milliseconds the system has been running for. Used for
   * timing (like millis() on Arduino).
   * TODO: should this live in a different class?
   */
  virtual uint16_t GetMs() = 0;

protected:
  std::vector<std::vector<uint8_t>> lightIds;
  int16_t centerLightRow;
  int16_t centerLightCol;
};

#endif
