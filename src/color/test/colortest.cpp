#include "gtest/gtest.h"

#include "../Color.hpp"

TEST(ColorTest, zeroSaturationAndValue) {
  HSV hsv = {HUE_RED, 0, 0};
  RGB expected = {0, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, zeroSaturation) {
  HSV hsv = {HUE_RED, 0, 255};
  RGB expected = {255, 255, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, zeroSaturationPartialValue) {
  HSV hsv = {HUE_RED, 0, 127};
  RGB expected = {127, 127, 127};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, red) {
  HSV hsv = {HUE_RED, 255, 255};
  RGB expected = {255, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, green) {
  HSV hsv = {HUE_GREEN, 255, 255};
  RGB expected = {0, 255, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blue) {
  HSV hsv = {HUE_BLUE, 255, 255};
  RGB expected = {0, 0, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blueDim) {
  HSV hsv = {HUE_BLUE, 255, 127};
  RGB expected = {0, 0, 127};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blueDesaturated) {
  HSV hsv = {HUE_BLUE, 127, 255};
  RGB expected = {128, 128, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellow) {
  HSV hsv = {HUE_YELLOW, 255, 255};
  RGB expected = {127, 127, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellowDesaturated) {
  HSV hsv = {HUE_YELLOW, 127, 255};
  RGB expected = {191, 191, 128};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellowDimDesaturated) {
  HSV hsv = {HUE_YELLOW, 127, 127};
  RGB expected = {94, 94, 63};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, hueOverflow) {
  HSV hsv = {720, 255, 255};
  RGB expected = {255, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
  EXPECT_EQ(hsv.h, 0);
}
