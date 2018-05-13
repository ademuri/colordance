#include "SimulatorLightController.hpp"
#include <OgreLight.h>
#include <OgreSceneNode.h>
#include <chrono>
#include <map>
#include <utility>

void SimulatorLightController::addGridLights(Ogre::SceneManager *scnMgr) {
  const int16_t xCenter = 0;
  const int16_t yCenter = inchesToCoords(24);
  const int16_t zCenter = feetToCoords(20);

  const int16_t xSpacing = inchesToCoords(5);
  const int16_t ySpacing = inchesToCoords(5);

  // Adds a 5x5 grid of lights
  int lightId = 1;
  for (int i = -2; i < 3; i++) {
    std::vector<int16_t> rowIds = {};
    for (int j = -2; j < 3; j++) {
      rowIds.push_back(lightId);
      lightIdMap[lightId] = createLight(Ogre::Vector3(
          xCenter + xSpacing * j, yCenter + ySpacing * i, zCenter));

      lightId++;
    }
    lightIds.push_back(rowIds);
  }

  numRows = 5;
  numCols = 5;
  centerLightCol = 3;
  centerLightRow = 3;
}

void SimulatorLightController::addIndividualLights(Ogre::SceneManager *scnMgr) {
  lightIds = {{0, 1, 0, 2, 3, 4, 0, 5, 0},
              {6, 7, 8, 9, 10, 11, 12, 13, 14},
              {0, 15, 0, 16, 17, 18, 0, 19, 0}};

  // These are the distances between the lights in a single block.
  const int16_t xOffset = inchesToCoords(12);
  const int16_t yOffset = inchesToCoords(24);

  const int16_t leftBlockX = feetToCoords(-5);
  const int16_t leftBlockY = inchesToCoords(24);
  const int16_t leftBlockZ = feetToCoords(20);

  const int16_t centerBlockX = feetToCoords(0);
  const int16_t centerBlockY = inchesToCoords(24);
  const int16_t centerBlockZ = feetToCoords(20);

  const int16_t rightBlockX = feetToCoords(5);
  const int16_t rightBlockY = inchesToCoords(24);
  const int16_t rightBlockZ = feetToCoords(20);

  lightIdMap[1] =
      createLight(Ogre::Vector3(leftBlockX, leftBlockY + yOffset, leftBlockZ));
  lightIdMap[6] =
      createLight(Ogre::Vector3(leftBlockX - xOffset, leftBlockY, leftBlockZ));
  lightIdMap[7] =
      createLight(Ogre::Vector3(leftBlockX, leftBlockY, leftBlockZ));
  lightIdMap[8] =
      createLight(Ogre::Vector3(leftBlockX + xOffset, leftBlockY, leftBlockZ));
  lightIdMap[15] =
      createLight(Ogre::Vector3(leftBlockX, leftBlockY - yOffset, leftBlockZ));

  lightIdMap[2] = createLight(Ogre::Vector3(
      centerBlockX - xOffset, centerBlockY + yOffset, centerBlockZ));
  lightIdMap[3] = createLight(
      Ogre::Vector3(centerBlockX, centerBlockY + yOffset, centerBlockZ));
  lightIdMap[4] = createLight(Ogre::Vector3(
      centerBlockX + xOffset, centerBlockY + yOffset, centerBlockZ));
  lightIdMap[9] = createLight(
      Ogre::Vector3(centerBlockX - xOffset, centerBlockY, centerBlockZ));
  lightIdMap[10] =
      createLight(Ogre::Vector3(centerBlockX, centerBlockY, centerBlockZ));
  lightIdMap[11] = createLight(
      Ogre::Vector3(centerBlockX + xOffset, centerBlockY, centerBlockZ));
  lightIdMap[16] = createLight(Ogre::Vector3(
      centerBlockX - xOffset, centerBlockY - yOffset, centerBlockZ));
  lightIdMap[17] = createLight(
      Ogre::Vector3(centerBlockX, centerBlockY - yOffset, centerBlockZ));
  lightIdMap[18] = createLight(Ogre::Vector3(
      centerBlockX + xOffset, centerBlockY - yOffset, centerBlockZ));

  lightIdMap[5] = createLight(
      Ogre::Vector3(rightBlockX, rightBlockY + yOffset, rightBlockZ));
  lightIdMap[12] = createLight(
      Ogre::Vector3(rightBlockX - xOffset, rightBlockY, rightBlockZ));
  lightIdMap[13] =
      createLight(Ogre::Vector3(rightBlockX, rightBlockY, rightBlockZ));
  lightIdMap[14] = createLight(
      Ogre::Vector3(rightBlockX + xOffset, rightBlockY, rightBlockZ));
  lightIdMap[19] = createLight(
      Ogre::Vector3(rightBlockX, rightBlockY - yOffset, rightBlockZ));

  centerLightRow = 1;
  centerLightCol = 4;

  numRows = 3;
  numCols = 9;
}

SimulatorLightController::SimulatorLightController(Ogre::SceneManager *scnMgr)
    : startTime(std::chrono::steady_clock::now()), scnMgr(scnMgr) {
  addGridLights(scnMgr);
}

void SimulatorLightController::Set(const int16_t lightId, HSV hsv) {
  if (lightId == 0) {
    // printf("Error: got 0 light id, hsv: {%d, %d, %d}\n", hsv.h, hsv.s,
    // hsv.v);
    return;
  }

  RGB rgb = Color::toRGB(hsv);
  lightIdMap[lightId]->setDiffuseColour(rgb.r / 255.0, rgb.g / 255.0,
                                        rgb.b / 255.0);
  lightIdMap[lightId]->setSpecularColour(rgb.r / 255.0, rgb.g / 255.0,
                                         rgb.b / 255.0);
}

long SimulatorLightController::GetMs() {
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
  spotLightNode->lookAt(Ogre::Vector3(0, 200, 0),
                        Ogre::Node::TransformSpace::TS_WORLD);

  // These angles are calculated from measurements on the light that I'll
  // probably use.
  // At 12" from a wall, the cone of well-mixed, bright white was 8", and the
  // cone of bright light (with color fringing) was 12".
  spotLight->setSpotlightRange(Ogre::Degree(36), Ogre::Degree(53));

  // Not actually measured, just a wild guess
  // TODO: tune this?
  spotLight->setSpotlightFalloff(10);

  // Make light get less bright farther away from the light sources.
  // TODO: tune this?
  spotLight->setAttenuation(2000, /* constant */ 1.0, /* linear */ 0.0005,
                            /* quadratic */ 0.0);

  return spotLight;
}

int SimulatorLightController::inchesToCoords(int inches) {
  // 64" == 5'4". Model is 192 units tall.
  return (inches * 192) / 64;
}

int SimulatorLightController::feetToCoords(int feet) {
  return inchesToCoords(12 * feet);
}
