#include "gtest/gtest.h"

#include <vector>
#include "../DummyParamController.hpp"
#include "../LightController.hpp"
#include "DummyLightController.hpp"

TEST(LightControllerTest, GetLights_3x3_one) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  p->Set(Params::kPan, ParamController::kPanNeutral);

  std::vector<std::vector<uint16_t>> expected = {{5}};
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_all) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  p->Set(Params::kPan, ParamController::kPanNeutral);

  std::vector<std::vector<uint16_t>> expected = {
      {1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  EXPECT_EQ(c.GetLights(p, 3, 3), expected);
}

TEST(LightControllerTest, GetLights_3x5_square) {
  // Light ids looks like this:
  // {1, 2, 3, 4, 5}
  // {6, 7, 8, 9, 10}
  // {11, 12, 13, 14, 15}
  DummyLightController c = DummyLightController(3, 5);
  DummyParamController *p = new DummyParamController();
  p->Set(Params::kPan, ParamController::kPanNeutral);

  std::vector<std::vector<uint16_t>> expected = {
      {2, 3, 4}, {7, 8, 9}, {12, 13, 14}};
  EXPECT_EQ(c.GetLights(p, 3, 3), expected);
}

TEST(LightControllerTest, GetLights_3x5_even) {
  // Light ids looks like this:
  // {1, 2, 3, 4, 5}
  // {6, 7, 8, 9, 10}
  // {11, 12, 13, 14, 15}
  DummyLightController c = DummyLightController(3, 5);
  DummyParamController *p = new DummyParamController();
  p->Set(Params::kPan, ParamController::kPanNeutral);

  std::vector<std::vector<uint16_t>> expected = {{3, 4}, {8, 9}};
  EXPECT_EQ(c.GetLights(p, 2, 2), expected);
}

TEST(LightControllerTest, GetLights_4x4_even) {
  // Light ids looks like this:
  // {1, 2, 3, 4}
  // {5, 6, 7, 8}
  // {9, 10, 11, 12}
  // {13, 14, 15, 16}
  DummyLightController c = DummyLightController(4, 4);
  DummyParamController *p = new DummyParamController();
  p->Set(Params::kPan, ParamController::kPanNeutral);

  std::vector<std::vector<uint16_t>> expected = {{6, 7}, {10, 11}};
  EXPECT_EQ(c.GetLights(p, 2, 2), expected);
}

TEST(LightControllerTest, GetLights_4x4_odd) {
  // Light ids looks like this:
  // {1, 2, 3, 4}
  // {5, 6, 7, 8}
  // {9, 10, 11, 12}
  // {13, 14, 15, 16}
  DummyLightController c = DummyLightController(4, 4);
  DummyParamController *p = new DummyParamController();
  p->Set(Params::kPan, ParamController::kPanNeutral);

  std::vector<std::vector<uint16_t>> expected = {{6, 7, 8}, {10, 11, 12}};
  EXPECT_EQ(c.GetLights(p, 2, 3), expected);
}

TEST(LightControllerTest, GetLights_3x3_pan) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();

  std::vector<std::vector<uint16_t>> expected = {{4}};
  p->Set(Params::kPan, 0);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);

  expected = {{5}};
  p->Set(Params::kPan, ParamController::kPanNeutral);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);

  expected = {{6}};
  p->Set(Params::kPan, 9);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_pan_multipleRows) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();

  std::vector<std::vector<uint16_t>> expected = {{1}, {4}, {7}};
  p->Set(Params::kPan, 0);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);

  expected = {{2}, {5}, {8}};
  p->Set(Params::kPan, ParamController::kPanNeutral);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);

  expected = {{3}, {6}, {9}};
  p->Set(Params::kPan, 9);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_pan_noRoomToPan) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();

  std::vector<std::vector<uint16_t>> expected = {{4, 5, 6}};
  p->Set(Params::kPan, 0);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);

  p->Set(Params::kPan, ParamController::kPanNeutral);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);

  p->Set(Params::kPan, 9);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);
}
