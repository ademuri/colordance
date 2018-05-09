#include "WProgram.h"
#include "DirectLightController.hpp"
#include "DirectParamController.hpp"
#include "../../src/controller/ColorShiftEffect.hpp"
#include "../../src/controller/StrobeEffect.hpp"

// This is some stuff to fix issues with the Teensy library vector implementation where
// it throws warnings or fails to link if there is no __throw_bad_alloc()
// or __throw_length_error() functions.

namespace std {
  void __throw_bad_alloc() {
    Serial.println("bad_alloc");
    while(1);
  }
  void __throw_length_error(char const *e) {
    Serial.print("length_error: ");
    Serial.println(e);
    while(1);
  }
}

extern "C" int main(void)
{
	pinMode(13, OUTPUT);
	/*while (1) {
		digitalWriteFast(13, HIGH);
		delay(500);
		digitalWriteFast(13, LOW);
		delay(500);
	}*/

  DirectLightController *lightController = new DirectLightController();
  ParamController *paramController = new DirectParamController();
  paramController->Set(Params::kHue0, 120);
  paramController->Set(Params::kTempo, 254);
  paramController->Set(Params::kWidth, 255);
  paramController->Set(Params::kPan, ParamController::kPanNeutral);
  paramController->Set(Params::kTilt, ParamController::kTiltNeutral);
  Effect *effect = new ColorShiftEffect(lightController, paramController);

  digitalWriteFast(13, HIGH);
  delay(500);
  digitalWriteFast(13, LOW);
  delay(500);
  
  /*lightController->Set(1, {0, 255, 255});
  lightController->Set(4, {0, 255, 255});
  lightController->Set(7, {0, 255, 255});*/

  bool lightStatus = false;
  while (1) {
    effect->Run();
		//digitalWriteFast(13, lightStatus);
    delay(5);

    lightStatus = !lightStatus;
  }
}

