#include "SimulatorLightController.hpp"
#include <OgreLight.h>
#include <map>
#include <utility>

SimulatorLightController::SimulatorLightController(Ogre::Light *left,
                                                   Ogre::Light *center,
                                                   Ogre::Light *right) {
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::STAGE_LEFT, left));
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::CENTER, center));
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::STAGE_RIGHT, right));
}

void SimulatorLightController::Set(const Lights light, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  lightMap[light]->setDiffuseColour(rgb.r / 255.0, rgb.g / 255.0,
                                    rgb.b / 255.0);
  lightMap[light]->setSpecularColour(rgb.r / 255.0, rgb.g / 255.0,
                                     rgb.b / 255.0);
}
