#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <cstdint>

/* RGB color. 8 bits per color, since that's what our lights support. */
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

inline bool operator==(const RGB &x, const RGB &y) {
  return x.r == y.r && x.g == y.g && x.b == y.b;
}

struct HSV {
  /* Hue. Ranges from 0 to 359. */
  uint16_t h;

  /* Saturation. Ranges from 0 to 255. */
  uint8_t s;

  /* Value. Ranges from 0 to 255. */
  uint8_t v;
};

enum Hue {
  HUE_RED = 0,
  HUE_YELLOW = 60,
  HUE_GREEN = 120,
  HUE_CYAN = 180,
  HUE_BLUE = 240,
  HUE_MAGENTA = 300,
};

class Color {
 public:
  static RGB toRGB(HSV &hsv);

 private:
  static uint8_t GetColorValue(Hue color, const HSV &hsv);

  // Multiply two numbers, clamping at 255.
  static uint8_t cmul(const uint8_t x, const uint8_t y);

  // The width of one primary color band - that is, starting with hue at 0
  // (pure red), hues 0 through 120 will contain some red.
  static const uint16_t kHueWidth = 120;
};
#endif
