#include <cstdio>
#include "../controller/BounceEffect.hpp"
#include "../controller/ColorShiftAndStrobeEffect.hpp"
#include "../controller/ColorShiftEffect.hpp"
#include "../controller/Effect.hpp"
#include "../controller/SolidColorEffect.hpp"
#include "../controller/StrobeEffect.hpp"
#include "../controller/ThreeColorEffect.hpp"
#include "../controller/UnevenSwitchStrobeEffect.hpp"
#include "LocalLightController.hpp"
#include "LocalParamController.hpp"

int main(int argc, char **argv) {
  LocalLightController *controller = new LocalLightController();
  ParamController *paramController = new LocalParamController();
  paramController->Set(Params::kHue0, 120);
  paramController->Set(Params::kTempo, 100);
  paramController->Set(Params::kWidth, 255);
  paramController->Set(Params::kPan, ParamController::kPanNeutral);
  paramController->Set(Params::kTilt, ParamController::kTiltNeutral);
  Effect *effect = new ColorShiftEffect(controller, paramController);

  uint16_t runAtMs = 0;
  while (1) {
    // Only run every 10ms so we don't overwhelm the DMX controller.
    if (controller->GetMs() > runAtMs) {
      effect->Run();
      controller->WriteDmx();
      runAtMs = controller->GetMs() + 5;
    }
  }

  return 0;
}
