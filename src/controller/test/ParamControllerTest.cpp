#include "gtest/gtest.h"

#include "../DummyParamController.hpp"

TEST(ParamControllerTest, GetScaled_scaleNoOp) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 255), 0);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 255), 255);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 255), 127);
}

TEST(ParamControllerTest, GetScaled_scalePositive) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 128), 0);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 128), 128);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 128), 64);
}

TEST(ParamControllerTest, GetScaled_roundsCorrectlyForSmallerRange) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 254);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 10), 10);

  controller.Set(Params::kTempo, 1);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 10), 0);
}

TEST(ParamControllerTest, GetScaled_evenlySpaced_evenBins) {
  // Range should be:
  // 0-127: 0
  // 128-255: 1
  DummyParamController controller;
  for (int i = 0; i <= 127; i++) {
    controller.Set(Params::kTempo, i);
    EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 1), 0)
        << "Should be 0 for " << i;
  }

  for (int i = 128; i <= 255; i++) {
    controller.Set(Params::kTempo, i);
    EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 1), 1)
        << "Should be 1 for " << i;
  }
}

TEST(ParamControllerTest, GetScaled_evenlySpaced_oddBins) {
  DummyParamController controller;
  for (int i = 0; i <= 84; i++) {
    controller.Set(Params::kTempo, i);
    EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 2), 0)
        << "Should be 0 for " << i;
  }

  for (int i = 85; i <= 169; i++) {
    controller.Set(Params::kTempo, i);
    EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 2), 1)
        << "Should be 1 for " << i;
  }

  for (int i = 170; i <= 255; i++) {
    controller.Set(Params::kTempo, i);
    EXPECT_EQ(controller.GetScaled(Params::kTempo, 0, 2), 2)
        << "Should be 2 for " << i;
  }
}

TEST(ParamControllerTest, GetScaled_negativeNoScale) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 255, 0), 255);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 255, 0), 0);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 255, 0), 128);
}

TEST(ParamControllerTest, GetScaled_scaleNegative) {
  DummyParamController controller;
  controller.Set(Params::kTempo, 0);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 128);

  controller.Set(Params::kTempo, 255);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 0);

  controller.Set(Params::kTempo, 127);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 64);

  controller.Set(Params::kTempo, 2);
  EXPECT_EQ(controller.GetScaled(Params::kTempo, 128, 0), 127);
}

TEST(ParamControllerTest, GetScaled_kHue0) {
  DummyParamController controller;
  controller.Set(Params::kHue0, 0);
  EXPECT_EQ(controller.GetScaled(Params::kHue0, 0, 255), 0);

  controller.Set(Params::kHue0, 359);
  EXPECT_EQ(controller.GetScaled(Params::kHue0, 0, 255), 255);
}

TEST(ParamControllerTest, SetScaled_sameRange) {
  DummyParamController controller;
  controller.SetScaled(Params::kTempo, 0, 0, 255);
  EXPECT_EQ(controller.Get(Params::kTempo), 0);

  controller.SetScaled(Params::kTempo, 255, 0, 255);
  EXPECT_EQ(controller.Get(Params::kTempo), 255);

  controller.SetScaled(Params::kTempo, 127, 0, 255);
  EXPECT_EQ(controller.Get(Params::kTempo), 127);
}

TEST(ParamControllerTest, SetScaled_smallerRange) {
  DummyParamController controller;
  controller.SetScaled(Params::kTempo, 0, 0, 10);
  EXPECT_EQ(controller.Get(Params::kTempo), 0);

  controller.SetScaled(Params::kTempo, 10, 0, 10);
  EXPECT_EQ(controller.Get(Params::kTempo), 255);

  controller.SetScaled(Params::kTempo, 5, 0, 10);
  EXPECT_EQ(controller.Get(Params::kTempo), 127);
}

TEST(ParamControllerTest, SetScaled_largerRange) {
  DummyParamController controller;
  controller.SetScaled(Params::kTempo, 0, 0, 1000);
  EXPECT_EQ(controller.Get(Params::kTempo), 0);

  controller.SetScaled(Params::kTempo, 1000, 0, 1000);
  EXPECT_EQ(controller.Get(Params::kTempo), 255);

  controller.SetScaled(Params::kTempo, 500, 0, 1000);
  EXPECT_EQ(controller.Get(Params::kTempo), 127);
}

TEST(ParamControllerTest, SetScaled_backwards) {
  DummyParamController controller;
  controller.SetScaled(Params::kTempo, 255, 255, 0);
  EXPECT_EQ(controller.Get(Params::kTempo), 0);

  controller.SetScaled(Params::kTempo, 0, 255, 0);
  EXPECT_EQ(controller.Get(Params::kTempo), 255);

  controller.SetScaled(Params::kTempo, 127, 255, 0);
  EXPECT_EQ(controller.Get(Params::kTempo), 128);
}

TEST(ParamControllerTest, SetScaled_backwardsNegative) {
  DummyParamController controller;
  controller.SetScaled(Params::kTempo, 127, 127, -128);
  EXPECT_EQ(controller.Get(Params::kTempo), 0);

  controller.SetScaled(Params::kTempo, -128, 127, -128);
  EXPECT_EQ(controller.Get(Params::kTempo), 255);

  controller.SetScaled(Params::kTempo, 0, 127, -128);
  EXPECT_EQ(controller.Get(Params::kTempo), 127);
}
