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
  virtual void Set(const int16_t lightId, HSV hsv) = 0;

  /*
   * Gets a block of lights with the specified dimensions. Lights aren't
   * guaranteed to be populated in each cell - nonexistent lights are indicated
   * by id 0.
   * TODO: make this respect parameters (e.g. width, panning).
   */
  std::vector<std::vector<int16_t>> GetLights(ParamController *paramController,
                                              int16_t rows, int16_t cols);

  /*
   * Gets a line of lights. The size and location will be based on the params.
   * Unlike GetLights, no null IDs will be present.
   */
  std::vector<int16_t> GetLightsFromParams(ParamController *paramController);

  /**
   * Like the above method, but with a specified width. Still uses params to do
   * pan/tilt/orientation. If width is 0, uses params to choose the width.
   */
  std::vector<int16_t> GetLightsFromParams(ParamController *paramController,
                                           unsigned int width);

  /*
   * Gets the number of milliseconds the system has been running for. Used for
   * timing (like millis() on Arduino).
   * TODO: should this live in a different class?
   */
  virtual unsigned long GetMs() = 0;

  /** Turns off all lights. */
  void Blackout();

  virtual void WriteLeds() {}

  /**
   * The decorative LED strips placed as part of the installation. These values
   * are written out when WriteLeds is called.
   */
  std::vector<RGB> leds;

  int16_t numRows = 0;
  int16_t numCols = 0;

 protected:
  /*
   * Two-dimensional array of light ids. The structure of this array reflects
   * the structure of the lights (spacing may not be even). The light ids are
   * used by implementations of this class to identify lights. For DMX, this
   * will probaby be the starting DMX id of the light.
   */
  std::vector<std::vector<int16_t>> lightIds;

  /* The physical center row of lightIds. */
  int16_t centerLightRow;

  /* The physical center col of lightIds. */
  int16_t centerLightCol;
};

#endif
