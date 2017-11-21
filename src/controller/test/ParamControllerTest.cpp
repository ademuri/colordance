#include "gtest/gtest.h"

#include "../DummyParamController.hpp"

TEST(ParamControllerTest, scaleNoOp) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 255), 0);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 255), 255);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 255), 127);
}

TEST(ParamControllerTest, scalePositive) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 128), 0);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 128), 128);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 128), 63);
}

TEST(ParamControllerTest, negativeNoScale) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 255, 0), 255);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 255, 0), 0);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 255, 0), 128);
}

TEST(ParamControllerTest, scaleNegative) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 128);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 0);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 65);

  controller.Set(Params::kTempo, 2);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 127);
}

TEST(ParamControllerTest, kHue0) {
  DummyParamController controller;
  controller.Set(Params::kHue0, 0);
  EXPECT_EQ(controller.GetScaled(Params::kHue0, 0, 255), 0);

  controller.Set(Params::kHue0, 359);
  EXPECT_EQ(controller.GetScaled(Params::kHue0, 0, 255), 255);
}
