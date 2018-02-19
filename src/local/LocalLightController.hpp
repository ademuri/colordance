#ifndef __LOCAL_LIGHT_CONTROLLER_HPP__
#define __LOCAL_LIGHT_CONTROLLER_HPP__

#include <chrono>
#include "../color/Color.hpp"
#include "../controller/LightController.hpp"

class LocalLightController : public LightController {
 public:
  LocalLightController() {}

  void Set(const uint16_t lightId, HSV hsv) override;
  uint16_t GetMs() override;

 private:
  /** Maps from public light id to DMX starting channel. */
  std::map<const uint16_t, const uint16_t> lightIdMap;

  const std::chrono::steady_clock::time_point startTime;
};
#endif
