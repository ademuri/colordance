#ifndef __DUMMY_LIGHT_CONTROLLER_HPP__
#define __DUMMY_LIGHT_CONTROLLER_HPP__

#include <vector>
#include "../../color/Color.hpp"
#include "../LightController.hpp"

/*
 * TODO
 */
class DummyLightController : public LightController {
 public:
  DummyLightController(const int8_t rows, const int8_t cols);

  long GetMs() override;

  void Set(const int16_t lightId, HSV hsv) override;

  HSV Get(const int16_t lightId);

 private:
  std::map<const int16_t, HSV> lights;
};

#endif
