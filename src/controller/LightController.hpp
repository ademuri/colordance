#ifndef __LIGHT_CONTROLLER_HPP__
#define __LIGHT_CONTROLLER_HPP__

#include <vector>
#include "../color/Color.hpp"
#include "ParamController.hpp"

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

  /*
   * Sets the specified light to an RGB value corresponding to the given HSV.
   */
  virtual void Set(const uint16_t lightId, HSV hsv) = 0;

  /*
   * Gets a block of lights with the specified dimensions. Lights aren't
   * guaranteed to be populated in each cell - nonexistent lights are indicated
   * by id 0.
   * TODO: make this respect parameters (e.g. width, panning).
   */
  virtual std::vector<std::vector<uint16_t>> GetLights(
      ParamController *paramController, int16_t rows, int16_t cols);

  /*
   * Gets the number of milliseconds the system has been running for. Used for
   * timing (like millis() on Arduino).
   * TODO: should this live in a different class?
   */
  virtual uint16_t GetMs() = 0;

  int16_t numRows = 0;
  int16_t numCols = 0;

 protected:
  /*
   * Two-dimensional array of light ids. The structure of this array reflects
   * the structure of the lights (spacing may not be even). The light ids are
   * used by implementations of this class to identify lights. For DMX, this
   * will probaby be the starting DMX id of the light.
   */
  std::vector<std::vector<uint16_t>> lightIds;

  /* The physical center row of lightIds. */
  int16_t centerLightRow;

  /* The physical center col of lightIds. */
  int16_t centerLightCol;
};

#endif
