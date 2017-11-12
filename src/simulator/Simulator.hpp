#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRoot.h>
#include <OgreLight.h>

class Simulator : public OgreBites::ApplicationContext,
                  public OgreBites::InputListener {
public:
  Simulator();

  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  void setup() override;

private:
  Ogre::Light *light1;
  Ogre::Light *light2;
  Ogre::Light *light3;
};

#endif
