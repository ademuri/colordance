#include <cstdint>

#include "Color.hpp"

/**
 * CIE 1931 lightness curve. Used to translate linear PWM to non-linear LEDs.
 * Generated using:
 * http://jared.geek.nz/2013/feb/linear-led-pwm
 */
const uint32_t cie[256] = {
    0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   7,   7,   7,   7,   8,   8,   8,   8,   9,   9,   9,   10,  10,
    10,  10,  11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  15,  15,
    15,  16,  16,  17,  17,  17,  18,  18,  19,  19,  20,  20,  21,  21,  22,
    22,  23,  23,  24,  24,  25,  25,  26,  26,  27,  28,  28,  29,  29,  30,
    31,  31,  32,  32,  33,  34,  34,  35,  36,  37,  37,  38,  39,  39,  40,
    41,  42,  43,  43,  44,  45,  46,  47,  47,  48,  49,  50,  51,  52,  53,
    54,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
    68,  70,  71,  72,  73,  74,  75,  76,  77,  79,  80,  81,  82,  83,  85,
    86,  87,  88,  90,  91,  92,  94,  95,  96,  98,  99,  100, 102, 103, 105,
    106, 108, 109, 110, 112, 113, 115, 116, 118, 120, 121, 123, 124, 126, 128,
    129, 131, 132, 134, 136, 138, 139, 141, 143, 145, 146, 148, 150, 152, 154,
    155, 157, 159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183,
    185, 187, 189, 191, 193, 196, 198, 200, 202, 204, 207, 209, 211, 214, 216,
    218, 220, 223, 225, 228, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255,
};

/**
 * Inverse gamma curve of 2.2 (i.e. gamma = 1 / 2.2), with an output range of
 * [0, 255] (2 bits). Used for saturation.
 * Generated using the script at:
 * https://learn.adafruit.com/led-tricks-gamma-correction/the-longer-fix
 */
const uint32_t invGamma8[256] = {
    0,   21,  28,  34,  39,  43,  46,  50,  53,  56,  59,  61,  64,  66,  68,
    70,  72,  74,  76,  78,  80,  82,  84,  85,  87,  89,  90,  92,  93,  95,
    96,  98,  99,  101, 102, 103, 105, 106, 107, 109, 110, 111, 112, 114, 115,
    116, 117, 118, 119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145,
    146, 147, 148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 156, 157, 158,
    159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 167, 168, 169, 170,
    170, 171, 172, 173, 173, 174, 175, 175, 176, 177, 178, 178, 179, 180, 180,
    181, 182, 182, 183, 184, 184, 185, 186, 186, 187, 188, 188, 189, 190, 190,
    191, 192, 192, 193, 194, 194, 195, 195, 196, 197, 197, 198, 199, 199, 200,
    200, 201, 202, 202, 203, 203, 204, 205, 205, 206, 206, 207, 207, 208, 209,
    209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215, 215, 216, 217, 217,
    218, 218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224, 224, 225, 225,
    226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233,
    233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240, 240,
    241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248,
    248, 249, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255,
    255,
};

uint8_t Color::cmul(const uint8_t x, const uint8_t y) {
  uint16_t val = x * y;
  if (val > 255) {
    val = 255;
  }
  return val;
}

uint8_t Color::GetColorValue(Hue color, const HSV &hsv) {
  int32_t offset = hsv.h - color;
  if (offset < 0) {
    offset = -offset;
  } else if (offset >= 240) {
    // This handles the wrap-around case
    offset = 360 - hsv.h;
  }

  const uint32_t invsat = 255 - invGamma8[hsv.s];
  // const uint16_t base = invsat * hsv.v / 255;
  const uint32_t base = (invsat * cie[hsv.v]);

  if (offset >= kHueWidth) {
    // Only saturation and value will effect the color
    // Shift by 12 bits:
    // 4 bits for the v gamma (since the output range is 12 bits)
    // 8 bits for the saturation
    return base / 255;
  }

  const uint32_t invoffset = kHueWidth - offset;
  return (base + ((invoffset * invGamma8[hsv.s] * cie[hsv.v]) / kHueWidth)) /
         255;
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
