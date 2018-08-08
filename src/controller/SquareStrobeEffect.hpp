#ifndef __SQUARE_STROBE_EFFECT_HPP__
#define __SQUARE_STROBE_EFFECT_HPP__

#include <algorithm>
#include <vector>
#include "Effect.hpp"
#include "LightController.hpp"

/*
 * Simple effect that has four lights in a square. The color of each
 * light is controllable by the user. The lights are chosen using the
 * width, pan, and tilt params.
 */
class SquareStrobeEffect : public Effect {
 public:
  SquareStrobeEffect(LightController *lightController,
                     ParamController *paramController);

  void BeatDetected() override;
  void ChooseLights() override;
  void ParamChanged(Params param) override;
  void RandomizeParams() override;

 protected:
  void DoRun() override;
  void SetLights() override;

 private:
  /** All lights used by this effect. */
  std::vector<int16_t> lightIds;

  /** The two banks of lights that the strobe alternates between. These are
   * indexes into lightIds (and hues). */
  std::vector<int16_t> aLights;
  std::vector<int16_t> bLights;

  /** The hues selected by the user. */
  std::vector<HSV> hues = {
      {0, 255, 255},
      {90, 255, 255},
      {180, 255, 255},
      {270, 255, 255},
  };

  /** Param for the tempo. 0 is no change, kMaxTempo is fast change. */
  int16_t tempo = 0;

  /** Tracks the state machine. */
  int32_t state = 0;

  /**
   * The time, in ms, when the next state change will occur. This effect doesn't
   * use SleepMs so that it can respond to the boost button.
   */
  unsigned long stateChangeAt = 0;

  const unsigned int kNumLights = 4;

  /** The inverse of the max tempo. */
  const int16_t kMaxTempo = 50;

  /** How long the lights are on for when strobing. Tempo controls the off
   * delay. */
  const int16_t kOnMs = 50;

  /**
   * Previous state of the boost button. Used to trigger a strobe on the rising
   * edge.
   */
  bool prevBoost = false;
};

#endif
