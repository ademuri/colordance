#ifndef __DIRECT_LIGHT_CONTROLLER_HPP__
#define __DIRECT_LIGHT_CONTROLLER_HPP__

#include "../../src/color/Color.hpp"
#include "../../src/controller/LightController.hpp"

// From https://github.com/ssilverman/TeensyDMX
#include "../libraries/TeensyDMX/src/TeensyDMX.h"

namespace teensydmx = ::qindesign::teensydmx;

class DirectLightController : public LightController {
 public:
  DirectLightController();

  void Set(const int16_t lightId, HSV hsv) override;
  unsigned long GetMs() override;
  void WriteLeds() override;

  /** Sets the state of the button lights. */
  void SetButtonLights(bool status);

 private:
  teensydmx::Sender dmx{Serial2};

  /** The number of serial LEDs. */
  // One onboard
  // 13 on the left side
  // 18 on the middle
  // 13 on the right
  // 6 on the top
  const int kNumLeds = 50;

  // Pins for the button LEDs.
  // TODO: make these individually controllable?
  const int kButtonLed1 = 35;
  const int kButtonLed2 = 36;
  const int kButtonLed3 = 37;
  const int kButtonLed4 = 38;
  const int kButtonLed5 = 22;
};

#endif
