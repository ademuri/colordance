#ifndef __PARAM_CONTROLLER_HPP__
#define __PARAM_CONTROLLER_HPP__

#include <map>

enum class Params {
  // Primary/first color hue.
  kHue0,

  // Other hues. These may or may not be used by a given effect.
  kHue1,
  kHue2,

  // TODO: add more hues and saturations/values

  // Relative speed. Interpreted differently by different effects. Smaller
  // values are slower.
  kTempo,

  // The width of effects - how many lights they'll use. Higher values mean use
  // more lights.
  // TODO: this isn't a good name, because we need parameters for how spread
  // out to be, and how many lights to use.
  kWidth,

  // The left-right shift.
  kPan,

  // The up-down shift.
  kTilt,

  // The orientation of chosen lights. Probably either horizontal or vertical.
  kOrientation,
};

/**
 * Interface class for controlling the params for effects. Eventually, there
 * will be a physical control surface for modifying these param.
 */
class ParamController {
 public:
  ParamController();

  /*
   * Gets the value of a param. Note that while the return type is uint16_t,
   * the semantic type could be an 8-bit value, a boolean, etc.
   */
  virtual uint16_t Get(Params param) = 0;

  /*
   * Gets a parameter scaled to the given range. min may be greater than max,
   * giving an inverted range.
   */
  int16_t GetScaled(Params param, int16_t min, int16_t max);

  /*
   * Returns the param, wrapped around to be in the appropriate range for the
   * given param.
   */
  uint16_t WrapParam(Params param, uint16_t val);

  /**
   * Sets the value of a param. I'm not sure yet if this will be useful
   * outside of the simulator. Possible usages are for automatically changing
   * params (e.g. if no one is interacting with the control surface).
   */
  virtual void Set(Params param, uint16_t val) = 0;

  /*
   * Sets the value of a param, scaled from 0 to the given max to the internal
   * param's range.
   */
  void SetScaled(Params param, int16_t val, int16_t min, int16_t max);

  // The "neutral" (center) values for kPan and kTilt.
  static const uint16_t kPanNeutral = 127;
  static const uint16_t kTiltNeutral = 127;

 private:
  std::map<const Params, int16_t> paramRangeMap;
};

#endif
