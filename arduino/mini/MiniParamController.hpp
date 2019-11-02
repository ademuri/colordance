#ifndef __MINI_PARAM_CONTROLLER_HPP__
#define __MINI_PARAM_CONTROLLER_HPP__

#include <set>
#include "../../src/controller/ParamController.hpp"
#include "WProgram.h"

/**
 * Reads params from the controls connected to this teensy.
 *
 * Also allows manually overriding the params (e.g. so that effects can be
 * controlled automatically if no-one is using the controls). In this case, each
 * param will stay as manually set until that particular control is changed.
 */
class MiniParamController : public ParamController {
 public:
  MiniParamController();

  int16_t Get(Params param) override;

  void Set(Params param, int16_t val) override;

  bool Boost() { return false; }

 private:
  std::map<const Params, int16_t> params;

  const std::set<Params> chooseLightParams = {
      Params::kWidth,       Params::kPan,    Params::kTilt,
      Params::kOrientation, Params::kParam2,
  };
};

#endif
