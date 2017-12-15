#include "gtest/gtest.h"

#include <vector>
#include "../DummyParamController.hpp"
#include "../LightController.hpp"
#include "DummyLightController.hpp"

void setDefaultParams(DummyParamController *p) {
  p->SetScaled(Params::kPan, 6, 0, 10);
  p->SetScaled(Params::kTilt, 5, 0, 10);
}

TEST(LightControllerTest, GetLights_3x3_one) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{5}};
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_all) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

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
  setDefaultParams(p);

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
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{2, 3}, {7, 8}};
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
  setDefaultParams(p);

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
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{6, 7, 8}, {10, 11, 12}};
  EXPECT_EQ(c.GetLights(p, 2, 3), expected);
}

TEST(LightControllerTest, GetLights_3x3_pan) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{4}};
  p->SetScaled(Params::kPan, 0, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);

  expected = {{5}};
  p->SetScaled(Params::kPan, 5, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);

  expected = {{6}};
  p->SetScaled(Params::kPan, 10, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_pan_multipleRows) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{1}, {4}, {7}};
  p->SetScaled(Params::kPan, 0, 0, 10);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);

  expected = {{2}, {5}, {8}};
  p->SetScaled(Params::kPan, 6, 0, 10);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);

  expected = {{3}, {6}, {9}};
  p->SetScaled(Params::kPan, 10, 0, 10);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_pan_noRoomToPan) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{4, 5, 6}};
  p->SetScaled(Params::kPan, 0, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);

  p->SetScaled(Params::kPan, 6, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);

  p->SetScaled(Params::kPan, 9, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);
}

TEST(LightControllerTest, GetLights_3x3_tilt) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{2}};
  p->SetScaled(Params::kTilt, 0, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);

  expected = {{5}};
  p->SetScaled(Params::kTilt, 5, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);

  expected = {{8}};
  p->SetScaled(Params::kTilt, 10, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_tilt_noRoomToTilt) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{2}, {5}, {8}};
  p->SetScaled(Params::kTilt, 0, 0, 10);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);

  p->SetScaled(Params::kTilt, 5, 0, 10);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);

  p->SetScaled(Params::kTilt, 9, 0, 10);
  EXPECT_EQ(c.GetLights(p, 3, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_tilt_multipleCols) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<std::vector<uint16_t>> expected = {{1, 2, 3}};
  p->SetScaled(Params::kTilt, 0, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);

  expected = {{4, 5, 6}};
  p->SetScaled(Params::kTilt, 5, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);

  expected = {{7, 8, 9}};
  p->SetScaled(Params::kTilt, 10, 0, 10);
  EXPECT_EQ(c.GetLights(p, 1, 3), expected);
}

TEST(LightControllerTest, GetLightsFromParams) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<uint16_t> expected = {5};
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {4, 5, 6};
  p->SetScaled(Params::kWidth, 10, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {1, 2, 3};
  p->SetScaled(Params::kTilt, 0, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {7, 8, 9};
  p->SetScaled(Params::kTilt, 10, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);
}

TEST(LightControllerTest, GetLightsFromParams_pan) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);

  std::vector<uint16_t> expected = {5};
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {4};
  p->SetScaled(Params::kPan, 0, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {6};
  p->SetScaled(Params::kPan, 10, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);
}

TEST(LightControllerTest, GetLightsFromParams_orientation) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();
  setDefaultParams(p);
  p->SetScaled(Params::kWidth, 10, 0, 10);

  std::vector<uint16_t> expected = {4, 5, 6};
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {2, 5, 8};
  p->SetScaled(Params::kOrientation, 1, 0, 1);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {1, 4, 7};
  p->SetScaled(Params::kTilt, 0, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {4, 7};
  p->SetScaled(Params::kWidth, 5, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);

  expected = {1, 4};
  p->SetScaled(Params::kPan, 0, 0, 10);
  EXPECT_EQ(c.GetLightsFromParams(p), expected);
}
