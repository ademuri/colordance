#include "gtest/gtest.h"

#include "../DummyParamController.hpp"
#include "../LightController.hpp"
#include "DummyLightController.hpp"
#include <vector>

TEST(LightControllerTest, GetLights_one) {
  DummyLightController c = DummyLightController();
  DummyParamController *p = new DummyParamController();

  std::vector<std::vector<uint8_t>> expected = {{5}};
  EXPECT_EQ(c.GetLights(p, 1, 1), expected);
}

TEST(LightControllerTest, GetLights_all) {
  DummyLightController c = DummyLightController();
  DummyParamController *p = new DummyParamController();

  std::vector<std::vector<uint8_t>> expected = {
      {1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  EXPECT_EQ(c.GetLights(p, 3, 3), expected);
}
