#include "gtest/gtest.h"

#include "../Color.hpp"

TEST(ColorTest, zeroSaturationAndValue) {
  HSV hsv = {C_HUE_RED, 0, 0};
  RGB expected = {0, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, zeroSaturation) {
  HSV hsv = {C_HUE_RED, 0, 255};
  RGB expected = {255, 255, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, zeroSaturationPartialValue) {
  HSV hsv = {C_HUE_RED, 0, 127};
  RGB expected = {47, 47, 47};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, red) {
  HSV hsv = {C_HUE_RED, 255, 255};
  RGB expected = {255, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, green) {
  HSV hsv = {C_HUE_GREEN, 255, 255};
  RGB expected = {0, 255, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blue) {
  HSV hsv = {C_HUE_BLUE, 255, 255};
  RGB expected = {0, 0, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blueDim) {
  HSV hsv = {C_HUE_BLUE, 255, 127};
  RGB expected = {0, 0, 47};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blueDesaturated) {
  HSV hsv = {C_HUE_BLUE, 127, 255};
  RGB expected = {69, 69, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellow) {
  HSV hsv = {C_HUE_YELLOW, 255, 255};
  RGB expected = {127, 127, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);

  hsv.h = 59;
  expected = {129, 125, 0};
  EXPECT_EQ(Color::toRGB(hsv), expected);

  hsv.h = 61;
  expected = {125, 129, 0};
  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellowDesaturated) {
  HSV hsv = {C_HUE_YELLOW, 127, 255};
  RGB expected = {162, 162, 69};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellowDimDesaturated) {
  HSV hsv = {C_HUE_YELLOW, 127, 127};
  RGB expected = {29, 29, 12};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, cyan) {
  HSV hsv = {C_HUE_CYAN, 255, 255};
  RGB expected = {0, 127, 127};

  EXPECT_EQ(Color::toRGB(hsv), expected);

  hsv.h = 179;
  expected = {0, 129, 125};
  EXPECT_EQ(Color::toRGB(hsv), expected);

  hsv.h = 181;
  expected = {0, 125, 129};
  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, magenta) {
  HSV hsv = {C_HUE_MAGENTA, 255, 255};
  RGB expected = {127, 0, 127};

  EXPECT_EQ(Color::toRGB(hsv), expected);

  hsv.h = 299;
  expected = {125, 0, 129};
  EXPECT_EQ(Color::toRGB(hsv), expected);

  hsv.h = 301;
  expected = {129, 0, 125};
  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, reddish) {
  HSV hsv = {1, 255, 255};
  RGB expected = {252, 2, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, moreReddish) {
  HSV hsv = {2, 255, 255};
  RGB expected = {250, 4, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, lowValueGammaToZero) {
  HSV hsv = {0, 255, 4};
  RGB expected = {0, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, lowValue) {
  HSV hsv = {0, 255, 5};
  RGB expected = {1, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, lowSaturation) {
  HSV hsv = {0, 1, 255};
  RGB expected = {255, 234, 234};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, hueOverflow) {
  HSV hsv = {720, 255, 255};
  RGB expected = {255, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
  EXPECT_EQ(hsv.h, 0);
}

TEST(ColorTest, interpolate_saturation) {
  HSV hsv1 = {0, 255, 0};
  HSV hsv2 = {0, 0, 0};
  HSV ret = Color::interpolate(hsv1, hsv2, 127);
  EXPECT_EQ(ret.s, 128);

  ret = Color::interpolate(hsv1, hsv2, 0);
  EXPECT_EQ(ret.s, 255);

  ret = Color::interpolate(hsv1, hsv2, 255);
  EXPECT_EQ(ret.s, 0);
}

TEST(ColorTest, interpolate_value) {
  HSV hsv1 = {0, 0, 0};
  HSV hsv2 = {0, 0, 255};
  HSV ret = Color::interpolate(hsv1, hsv2, 127);
  EXPECT_EQ(ret.v, 127);

  ret = Color::interpolate(hsv1, hsv2, 0);
  EXPECT_EQ(ret.v, 0);

  ret = Color::interpolate(hsv1, hsv2, 255);
  EXPECT_EQ(ret.v, 255);
}

TEST(ColorTest, interpolate_hue_smallInterval) {
  HSV hsv1 = {0, 0, 0};
  HSV hsv2 = {10, 0, 0};
  HSV ret = Color::interpolate(hsv1, hsv2, 127);
  EXPECT_EQ(ret.h, 4);

  ret = Color::interpolate(hsv1, hsv2, 0);
  EXPECT_EQ(ret.h, 0);

  ret = Color::interpolate(hsv1, hsv2, 255);
  EXPECT_EQ(ret.h, 10);
}

TEST(ColorTest, interpolate_hue_wrapAround) {
  HSV hsv1 = {355, 0, 0};
  HSV hsv2 = {5, 0, 0};
  HSV ret = Color::interpolate(hsv1, hsv2, 127);
  EXPECT_EQ(ret.h, 359);

  ret = Color::interpolate(hsv1, hsv2, 0);
  EXPECT_EQ(ret.h, 355);

  ret = Color::interpolate(hsv1, hsv2, 255);
  EXPECT_EQ(ret.h, 5);
}
