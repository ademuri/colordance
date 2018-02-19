#ifndef __LOCAL_PARAM_CONTROLLER_HPP__
#define __LOCAL_PARAM_CONTROLLER_HPP__

#include <map>
#include "../controller/ParamController.hpp"

/** Reads params from a serial port, if present. Allows direct reading and setting params. */
class LocalParamController : public ParamController {
 public:
  LocalParamController();

  uint16_t Get(Params param) override;

  void Set(Params param, uint16_t val) override;

 private:
  std::map<const Params, uint16_t> params;
};

#endif

