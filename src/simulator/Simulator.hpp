#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include "../controller/SolidColorEffect.hpp"
#include "SimulatorLightController.hpp"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreLight.h>
#include <OgreRoot.h>

class Simulator : public OgreBites::ApplicationContext,
                  public OgreBites::InputListener {
public:
  Simulator();

  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  void setup() override;
  bool frameEnded(const Ogre::FrameEvent &evt) override;

private:
  SimulatorLightController *controller;
  SolidColorEffect *solidColorEffect;
};

#endif
