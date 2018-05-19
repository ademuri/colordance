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

  // These two are different per-effect. They're the left two pots.
  kParam1,
  kParam2,
};

/**
 * Interface class for controlling the params for effects. Eventually, there
 * will be a physical control surface for modifying these param.
 */
class ParamController {
 public:
  ParamController();

  /*
   * Gets the value of a param. Note that while the return type is int16_t,
   * the semantic type could be an 8-bit value, a boolean, etc.
   */
  virtual int16_t Get(Params param) = 0;

  /*
   * Gets a parameter scaled to the given range. min may be greater than max,
   * giving an inverted range.
   */
  int16_t GetScaled(Params param, int16_t min, int16_t max);

  /*
   * Returns the param, wrapped around to be in the appropriate range for the
   * given param.
   */
  int16_t WrapParam(Params param, int16_t val);

  /**
   * Sets the value of a param. I'm not sure yet if this will be useful
   * outside of the simulator. Possible usages are for automatically changing
   * params (e.g. if no one is interacting with the control surface).
   */
  virtual void Set(Params param, int16_t val) = 0;

  /*
   * Sets the value of a param, scaled from 0 to the given max to the internal
   * param's range.
   */
  void SetScaled(Params param, int16_t val, int16_t min, int16_t max);

  // The "neutral" (center) values for kPan and kTilt.
  static const int16_t kPanNeutral = 127;
  static const int16_t kTiltNeutral = 127;

  std::map<const Params, const int16_t> paramRangeMap = {
      {Params::kHue0, 96},    {Params::kHue1, 96},
      {Params::kHue2, 96},    {Params::kTempo, 255},
      {Params::kWidth, 255},  {Params::kPan, 255},
      {Params::kTilt, 255},   {Params::kOrientation, 255},
      {Params::kParam1, 255}, {Params::kParam2, 255},

  };
};

#endif
