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
  Ogre::Light *light1;
  Ogre::Light *light2;
  Ogre::Light *light3;

  SimulatorLightController *controller;
  SolidColorEffect *solidColorEffect;
};

#endif
