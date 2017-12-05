#include <cstdint>

#include "Color.hpp"

uint8_t Color::cmul(const uint8_t x, const uint8_t y) {
  uint16_t val = x * y;
  if (val > 255) {
    val = 255;
  }
  return val;
}

uint8_t Color::GetColorValue(Hue color, const HSV &hsv) {
  int16_t offset = hsv.h - color;
  if (offset < 0) {
    offset = -offset;
  } else if (offset >= 240) {
    // This handles the wrap-around case
    offset = 360 - hsv.h;
  }

  const uint8_t invsat = 255 - hsv.s;
  const uint16_t base = invsat * hsv.v / 255;

  if (offset >= kHueWidth) {
    // Only saturation and value will effect the color
    return base;
  }

  const uint16_t invoffset = kHueWidth - offset;
  return base + (invoffset * hsv.s / kHueWidth) * hsv.v / 255;
}

RGB Color::toRGB(HSV &hsv) {
  RGB rgb;

  // TODO: is it bad to "fix" hsv.h here?
  hsv.h = hsv.h % 360;

  rgb.r = GetColorValue(HUE_RED, hsv);
  rgb.g = GetColorValue(HUE_GREEN, hsv);
  rgb.b = GetColorValue(HUE_BLUE, hsv);

  return rgb;
}
