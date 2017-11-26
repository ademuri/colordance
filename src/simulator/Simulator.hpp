#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreLight.h>
#include <OgreRoot.h>
#include <map>
#include <string>
#include <vector>
#include "../controller/Effect.hpp"
#include "../controller/ParamController.hpp"
#include "SimulatorLightController.hpp"

enum class ControlKeys {
  kUp,
  kDown,
};

class Simulator : public OgreBites::ApplicationContext,
                  public OgreBites::InputListener {
 public:
  Simulator();

  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  bool keyReleased(const OgreBites::KeyboardEvent &evt) override;
  void setup() override;
  bool frameEnded(const Ogre::FrameEvent &evt) override;

 private:
  SimulatorLightController *controller;
  ParamController *paramController;
  Effect *effect;

  // Keeps track of whether keys are pressed down
  std::map<ControlKeys, bool> keyDownMap;

  // Counter to only check the arrow key state every so often, so that when
  // held down they don't change params too fast. This is a janky, but simple
  // way of doing it, because OgreBites doesn't appear to do this for you.
  uint16_t keyDownDebounce = 0;

  // Used to allow manually typing in the number value of tempo. Accumulates
  // numbers when they're pressed, and is cleared upon pressing ENTER.
  std::string numberInputBuffer;

  // The list of params that are adjustable with the arrow keys
  std::vector<Params> adjustableParams = {
      Params::kTempo, Params::kWidth, Params::kPan,
  };

  // Which param in the above vector is currently being adjusted
  int16_t currentParamIndex = 0;
};

#endif
