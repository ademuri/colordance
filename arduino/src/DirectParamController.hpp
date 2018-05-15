#ifndef __DIRECT_PARAM_CONTROLLER_HPP__
#define __DIRECT_PARAM_CONTROLLER_HPP__

#include <Encoder.h>
#include <map>
#include <set>
#include "../../src/controller/Effect.hpp"
#include "../../src/controller/ParamController.hpp"
#include "WProgram.h"

enum class ParamChanged {
  /** No params changed */
  kNone,
  /** A param changed that affects which lights are selected */
  kChooseLights,
  /** A param changed, but not one that affects light selection */
  kOther,
};

/**
 * Reads params from the controls connected to this teensy.
 *
 * Also allows manually overriding the params (e.g. so that effects can be
 * controlled automatically if no-one is using the controls). In this case, each
 * param will stay as manually set until that particular control is changed.
 */
class DirectParamController : public ParamController {
 public:
  DirectParamController();

  int16_t Get(Params param) override;

  void Set(Params param, int16_t val) override;

  ParamChanged ScanForChanges(Effect *effect);

 private:
  std::map<const Params, int16_t> params;

  /** Maps from param to the pot pin */
  const std::map<const Params, const int> potParamMap = {
      {Params::kWidth, 14}, {Params::kPan, 16},     {Params::kTilt, 23},
      {Params::kTempo, 17}, {Params::kParam1, A21}, {Params::kParam2, A22},
  };

  /** Holds the most recently read values of the pots. Used to determine if a
   * control has changed - if it has, then we set the param. This allows
   * programmatic control of the params. */
  std::map<const Params, int16_t> potValueMap;

  std::map<const Params, Encoder *const> encoderParamMap = {
      {Params::kHue0, new Encoder(6, 7)},
      {Params::kHue1, new Encoder(4, 5)},
      {Params::kHue2, new Encoder(2, 3)},
      // TODO: remove this and make it controlled by buttons instead
      {Params::kEffect, new Encoder(8, 9)},
  };

  const std::set<Params> chooseLightParams = {
      Params::kWidth, Params::kPan, Params::kTilt, Params::kOrientation,
  };

  // Pins
  const int kWidthPin = A2;
};

#endif
