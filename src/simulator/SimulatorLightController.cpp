#include "SimulatorLightController.hpp"
#include <OgreLight.h>
#include <chrono>
#include <map>
#include <utility>

SimulatorLightController::SimulatorLightController(Ogre::Light *left,
                                                   Ogre::Light *center,
                                                   Ogre::Light *right,
                                                   Ogre::Light *top,
                                                   Ogre::Light *bottom)
    : startTime(std::chrono::steady_clock::now()) {
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::STAGE_LEFT, left));
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::CENTER, center));
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::STAGE_RIGHT, right));
  lightMap.insert(std::pair<const Lights, Ogre::Light *>(Lights::TOP, top));
  lightMap.insert(
      std::pair<const Lights, Ogre::Light *>(Lights::BOTTOM, bottom));
}

void SimulatorLightController::Set(const Lights light, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  lightMap[light]->setDiffuseColour(rgb.r / 255.0, rgb.g / 255.0,
                                    rgb.b / 255.0);
  lightMap[light]->setSpecularColour(rgb.r / 255.0, rgb.g / 255.0,
                                     rgb.b / 255.0);
}

uint16_t SimulatorLightController::GetMs() {
  const std::chrono::steady_clock::time_point endTime =
      std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                               startTime)
      .count();
}
