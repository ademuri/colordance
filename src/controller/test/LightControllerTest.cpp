#include "gtest/gtest.h"

#include "../DummyParamController.hpp"
#include "../LightController.hpp"
#include "DummyLightController.hpp"
#include <vector>

TEST(LightControllerTest, GetLights_3x3_one) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();

  std::vector<std::vector<uint16_t>> expected = {{5}};
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_3x3_all) {
  DummyLightController c = DummyLightController(3, 3);
  DummyParamController *p = new DummyParamController();

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

  std::vector<std::vector<uint16_t>> expected = {{6, 7, 8}, {10, 11, 12}};
  EXPECT_EQ(c.GetLights(p, 2, 3), expected);
}
