#ifndef __DIRECT_PARAM_CONTROLLER_HPP__
#define __DIRECT_PARAM_CONTROLLER_HPP__

#include <map>
#include "../../src/controller/ParamController.hpp"

/**
 * Reads params the controls connected to this teensy.
 *
 * TODO: actually do that
 */
class DirectParamController : public ParamController {
 public:
  DirectParamController();

  uint16_t Get(Params param) override;

  void Set(Params param, uint16_t val) override;

 private:
  std::map<const Params, uint16_t> params;

  // Pins
  const int kWidthPin = A2;
};

#endif
