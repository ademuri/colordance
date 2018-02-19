#include "LocalLightController.hpp"
#include "LocalParamController.hpp"
#include "../controller/Effect.hpp"
#include "../controller/ThreeColorEffect.hpp"

int main(int argc, char **argv) {
  LocalLightController *controller = new LocalLightController();
  ParamController *paramController = new LocalParamController();
  paramController->Set(Params::kHue0, 120);
  paramController->Set(Params::kTempo, 200);
  paramController->Set(Params::kWidth, 5);
  paramController->Set(Params::kPan, ParamController::kPanNeutral);
  paramController->Set(Params::kTilt, ParamController::kTiltNeutral);
  Effect *effect = new ThreeColorEffect(controller, paramController);

  while (1) {
    effect->Run();
  }

  return 0;
}
