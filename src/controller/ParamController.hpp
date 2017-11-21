#ifndef __PARAM_CONTROLLER_HPP__
#define __PARAM_CONTROLLER_HPP__

enum class Params {
  // Primary/first color hue.
  kHue0,
};

/**
 * Interface class for controlling the params for effects. Eventually, there
 * will be a physical control surface for modifying these param.
 */
class ParamController {
public:
  ParamController() {}

  /*
   * Gets the value of a param. Note that while the return type is uint16_t,
   * the semantic type could be an 8-bit value, a boolean, etc.
   */
  virtual uint16_t Get(Params param) = 0;

  /**
   * Sets the value of a param. I'm not sure yet if this will be useful
   * outside of the simulator. Possible usages are for automatically changing
   * params (e.g. if no one is interacting with the control surface).
   */
  virtual void Set(Params param, uint16_t val) = 0;
};

#endif
