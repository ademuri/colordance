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
  RGB expected = {55, 55, 55};

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
  RGB expected = {0, 0, 55};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, blueDesaturated) {
  HSV hsv = {HUE_BLUE, 127, 255};
  RGB expected = {69, 69, 255};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellow) {
  HSV hsv = {HUE_YELLOW, 255, 255};
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
  HSV hsv = {HUE_YELLOW, 127, 255};
  RGB expected = {162, 162, 69};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, yellowDimDesaturated) {
  HSV hsv = {HUE_YELLOW, 127, 127};
  RGB expected = {34, 34, 14};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, cyan) {
  HSV hsv = {HUE_CYAN, 255, 255};
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
  HSV hsv = {HUE_MAGENTA, 255, 255};
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
  HSV hsv = {0, 255, 14};
  RGB expected = {0, 0, 0};

  EXPECT_EQ(Color::toRGB(hsv), expected);
}

TEST(ColorTest, lowValue) {
  HSV hsv = {0, 255, 15};
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
