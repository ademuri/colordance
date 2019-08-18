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

  /** Current tempo. Bigger is slower. */
  int16_t tempo = 1000;

  /**
   * The tempo according to the params. Used so that we can double the tempo when boost is pressed.
   */
  int16_t tempoFromParams = 1000;

  /**
   * Time used for sin calculations. Use this instead of GetMs so
   * that the lights don't flicker when changing the tempo.
   */
  float time = 0;

  /** Tracks the state machine. */
  int32_t state = 0;
};

#endif
