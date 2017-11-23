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

  lightIds = {{0, 1, 0}, {2, 3, 4}, {0, 5, 0}};

  lightIdMap[1] = top;
  lightIdMap[2] = left;
  lightIdMap[3] = center;
  lightIdMap[4] = right;
  lightIdMap[5] = bottom;

  centerLightRow = 1;
  centerLightCol = 1;
}

void SimulatorLightController::Set(const Lights light, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  lightMap[light]->setDiffuseColour(rgb.r / 255.0, rgb.g / 255.0,
                                    rgb.b / 255.0);
  lightMap[light]->setSpecularColour(rgb.r / 255.0, rgb.g / 255.0,
                                     rgb.b / 255.0);
}

void SimulatorLightController::Set(const uint16_t lightId, HSV hsv) {
  if (lightId == 0) {
    printf("Error: got 0 light id, hsv: {%d, %d, %d}\n", hsv.h, hsv.s, hsv.v);
    return;
  }

  RGB rgb = Color::toRGB(hsv);
  lightIdMap[lightId]->setDiffuseColour(rgb.r / 255.0, rgb.g / 255.0,
                                        rgb.b / 255.0);
  lightIdMap[lightId]->setSpecularColour(rgb.r / 255.0, rgb.g / 255.0,
                                         rgb.b / 255.0);
}

uint16_t SimulatorLightController::GetMs() {
  const std::chrono::steady_clock::time_point endTime =
      std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                               startTime)
      .count();
}
