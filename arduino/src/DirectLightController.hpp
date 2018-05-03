#ifndef __DIRECT_LIGHT_CONTROLLER_HPP__
#define __DIRECT_LIGHT_CONTROLLER_HPP__

#include "../../src/color/Color.hpp"
#include "../../src/controller/LightController.hpp"

// From https://github.com/ssilverman/TeensyDMX
#include "TeensyDMX.h"

namespace teensydmx = ::qindesign::teensydmx;

class DirectLightController : public LightController {
  public:
    DirectLightController();

    void Set(const uint16_t lightId, HSV hsv) override;
    uint16_t GetMs() override;

  private:
    teensydmx::Sender dmx{Serial2};

};

#endif
