#ifndef __SIMULATOR_LIGHT_CONTROLLER_HPP__
#define __SIMULATOR_LIGHT_CONTROLLER_HPP__

#include <OgreLight.h>
#include <OgreSceneManager.h>
#include <chrono>
#include <map>
#include "../color/Color.hpp"
#include "../controller/LightController.hpp"

/*
 * Interface class for controlling the output lights. Eventually, there will be
 * a subclass of this that uses DMX to control real lights.
 */
class SimulatorLightController : public LightController {
 public:
  SimulatorLightController(Ogre::SceneManager *scnMgr);

  void Set(const int16_t lightId, HSV hsv) override;

  unsigned long GetMs() override;

  /*
   * Converts inches to the x-y-z coordinates used by Ogre. This is all
   * relative to the height of the ninja mesh used, which is 192 coordinate
   * units tall. This assumes that the ninja is 5'4" (average human height).
   */
  static int inchesToCoords(int inches);
  static int feetToCoords(int inches);

 private:
  Ogre::Light *createLight(Ogre::Vector3 const position);
  void addIndividualLights(Ogre::SceneManager *scnMgr);
  void addGridLights(Ogre::SceneManager *scnMgr);

  std::map<const Lights, Ogre::Light *> lightMap;
  std::map<const int16_t, Ogre::Light *> lightIdMap;

  const std::chrono::steady_clock::time_point startTime;
  Ogre::SceneManager *const scnMgr;
};

#endif
