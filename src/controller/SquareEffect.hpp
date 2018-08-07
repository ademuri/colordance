#ifndef __SQUARE_EFFECT_HPP__
#define __SQUARE_EFFECT_HPP__

#include <algorithm>
#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

/*
 * Simple effect that has four lights in a square. The color of each
 * light is controllable by the user. The lights are chosen using the
 * width, pan, and tilt params.
 */
class SquareEffect : public Effect {
 public:
  SquareEffect(LightController *lightController,
               ParamController *paramController);

  void BeatDetected() override;
  void ChooseLights() override;
  void ParamChanged(Params param) override;
  void RandomizeParams() override;

 protected:
  /* Called periodically to perform the effect. */
  void DoRun() override;

 private:
  std::vector<int16_t> lightIds;

  /** The hues selected by the user. */
  std::vector<HSV> baseHues = {
      {0, 255, 255},
      {90, 255, 255},
      {180, 255, 255},
      {270, 255, 255},
  };

  /**
   * The rotated hues. If tempo is non-zero, these will interpolate between the
   * base hues.
   */
  std::vector<HSV> actualHues = baseHues;

  /** Param for the tempo. 0 is no change, kMaxTempo is fast change. */
  int16_t tempo = 0;

  /** Which direction the hue is currently changing (positive or negative) */
  int16_t hueDirection = 1;

  /** The oscillator that changes hues. Varies between 0 and kBandWidth. */
  int16_t hueOsc = 0;

  /** How wide the colors vary. */
  int16_t hueBandWidth = 60;

  const unsigned int kNumLights = 4;

  /** How many shiftPosition steps between each color when interpolating. */
  const int16_t kStepsPerHue = 100;

  /** The inverse of the max tempo. */
  const int16_t kMaxTempo = 50;
};

#endif
