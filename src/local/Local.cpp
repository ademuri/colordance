#include "../controller/Effect.hpp"
#include "../controller/SolidColorEffect.hpp"
#include "../controller/ThreeColorEffect.hpp"
#include "LocalLightController.hpp"
#include "LocalParamController.hpp"

int main(int argc, char **argv) {
  LocalLightController *controller = new LocalLightController();
  ParamController *paramController = new LocalParamController();
  paramController->Set(Params::kHue0, 120);
  paramController->Set(Params::kTempo, 200);
  paramController->Set(Params::kWidth, 255);
  paramController->Set(Params::kPan, ParamController::kPanNeutral);
  paramController->Set(Params::kTilt, ParamController::kTiltNeutral);
  Effect *effect = new SolidColorEffect(controller, paramController);

  effect->Run();

  return 0;
}
