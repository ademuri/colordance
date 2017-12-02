#include "gtest/gtest.h"

#include "../DummyParamController.hpp"
#include "../ParamController.hpp"
#include "../SolidColorEffect.hpp"
#include "DummyLightController.hpp"

void expectValue(DummyLightController *const lightController,
                 const std::vector<uint16_t> &lightIds, const HSV &hsv) {
  std::vector<uint16_t>::const_iterator it;
  for (it = lightIds.begin(); it < lightIds.end(); it++) {
    EXPECT_EQ(lightController->Get(*it), hsv) << "Expected light " << *it;
  }
}

void expectOff(DummyLightController *const lightController,
               const std::vector<uint16_t> &lightIds) {
  const HSV off = {0, 0, 0};
  expectValue(lightController, lightIds, off);
}

void expectOn(DummyLightController *const lightController,
              const std::vector<uint16_t> &lightIds, uint16_t onVal) {
  std::vector<uint16_t>::const_iterator it;
  for (it = lightIds.begin(); it < lightIds.end(); it++) {
    EXPECT_EQ(lightController->Get(*it).v, onVal)
        << "Expected light " << *it << " to have value " << onVal;
  }
}

TEST(SolidColorEffectTest, turnsOffOldLights) {
  DummyLightController *lightController = new DummyLightController(3, 3);
  DummyParamController *paramController = new DummyParamController();
  SolidColorEffect *effect =
      new SolidColorEffect(lightController, paramController);
  const HSV on = {0, 255, 255};

  paramController->SetScaled(Params::kWidth, 0, 0, 10);
  paramController->SetScaled(Params::kPan, 6, 0, 10);
  paramController->SetScaled(Params::kTilt, 5, 0, 10);
  effect->ChooseLights();
  effect->Run();

  // Params are set so that the effect chooses only the middle light
  // Light ids look like this:
  // 1 2 3
  // 4 5 6
  // 7 8 9
  expectOff(lightController, {1, 2, 3, 4, 6, 7, 8, 9});
  expectValue(lightController, {5}, on);

  // Change which light is selected and verify that the old light turns off
  paramController->SetScaled(Params::kPan, 10, 0, 10);
  effect->ChooseLights();
  effect->Run();
  expectOff(lightController, {1, 2, 3, 4, 5, 7, 8, 9});
  expectValue(lightController, {6}, on);

  // Change but don't exclude the old light, so it shouldn't turn off.
  paramController->SetScaled(Params::kWidth, 10, 0, 10);
  effect->ChooseLights();
  effect->Run();
  expectOff(lightController, {1, 2, 3, 7, 8, 9});
  expectOn(lightController, {4, 5, 6}, 151);
}
