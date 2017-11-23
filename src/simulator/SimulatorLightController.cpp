#include "SimulatorLightController.hpp"
#include <OgreLight.h>
#include <OgreSceneNode.h>
#include <chrono>
#include <map>
#include <utility>

SimulatorLightController::SimulatorLightController(Ogre::SceneManager *scnMgr)
    : startTime(std::chrono::steady_clock::now()), scnMgr(scnMgr) {
  lightIds = {{0, 1, 0}, {2, 3, 4}, {0, 5, 0}};

  lightIdMap[1] = createLight(Ogre::Vector3(0, 100, 900));
  lightIdMap[2] = createLight(Ogre::Vector3(-100, 100, 800));
  lightIdMap[3] = createLight(Ogre::Vector3(0, 100, 800));
  lightIdMap[4] = createLight(Ogre::Vector3(100, 100, 800));
  lightIdMap[5] = createLight(Ogre::Vector3(0, 100, 700));

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

Ogre::Light *SimulatorLightController::createLight(
    Ogre::Vector3 const position) {
  Ogre::Light *spotLight = scnMgr->createLight();
  spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
  spotLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
  // Default lights to off
  spotLight->setDiffuseColour(0, 0, 0);
  spotLight->setSpecularColour(0, 0, 0);

  Ogre::SceneNode *spotLightNode =
      scnMgr->getRootSceneNode()->createChildSceneNode();
  spotLightNode->attachObject(spotLight);
  spotLightNode->setDirection(0, 0, -1);
  spotLightNode->setPosition(position);

  // These angles are calculated from measurements on the light that I'll
  // probably use.
  // At 12" from a wall, the cone of well-mixed, bright white was 8", and the
  // cone of bright light (with color fringing) was 12".
  spotLight->setSpotlightRange(Ogre::Degree(36), Ogre::Degree(53));

  return spotLight;
}
