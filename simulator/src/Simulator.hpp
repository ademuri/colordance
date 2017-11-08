#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRoot.h>

class Simulator : public OgreBites::ApplicationContext,
                  public OgreBites::InputListener {
public:
  Simulator();

  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  void setup() override;
};

#endif
