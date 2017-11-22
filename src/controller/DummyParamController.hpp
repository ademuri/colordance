#ifndef __DUMMY_PARAM_CONTROLLER_HPP__
#define __DUMMY_PARAM_CONTROLLER_HPP__

#include <map>
#include "ParamController.hpp"

class DummyParamController : public ParamController {
 public:
  DummyParamController();

  uint16_t Get(Params param) override;

  void Set(Params param, uint16_t val) override;

 private:
  std::map<const Params, uint16_t> params;
};

#endif
