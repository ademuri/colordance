#ifndef __CHROMATIC_ABBERATION__
#define __CHROMATIC_ABBERATION__

#include "Effect.hpp"
#include "LightController.hpp"

class ChromaticAberrationEffect : public Effect {
 public:
  ChromaticAberrationEffect(LightController *lightController,
                            ParamController *paramController);

  void BeatDetected() override {}

  void ChooseLights() override;

  void ParamChanged(Params param) override;

 protected:
  void DoRun() override;

 private:
  uint16_t getHue(float pos);
  uint16_t getSaturation(float pos);

  /** The center of the effect defined [0, 1] */
  float c = 0.5;

  /** The width of the effect */
  float w = 0.25;

  std::vector<uint16_t> lightIds;
};

#endif
