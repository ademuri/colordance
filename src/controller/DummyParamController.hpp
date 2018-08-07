#ifndef __DUMMY_PARAM_CONTROLLER_HPP__
#define __DUMMY_PARAM_CONTROLLER_HPP__

#include <map>
#include "ParamController.hpp"

class DummyParamController : public ParamController {
 public:
  DummyParamController();

  int16_t Get(Params param) override;

  void Set(Params param, int16_t val) override;

  bool Boost() override;

 private:
  std::map<const Params, int16_t> params;
};

#endif
