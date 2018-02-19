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
  DummyLightController(const uint8_t rows, const uint8_t cols);

  uint16_t GetMs() override;

  void Set(const uint16_t lightId, HSV hsv) override;

  HSV Get(const uint16_t lightId);

 private:
  std::map<const uint16_t, HSV> lights;
};

#endif
