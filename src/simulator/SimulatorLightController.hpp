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

  void Set(const Lights light, HSV hsv) override;

  void Set(const uint16_t lightId, HSV hsv) override;

  uint16_t GetMs() override;

 private:
  Ogre::Light *createLight(Ogre::Vector3 const position);

  std::map<const Lights, Ogre::Light *> lightMap;
  std::map<const uint16_t, Ogre::Light *> lightIdMap;

  const std::chrono::steady_clock::time_point startTime;
  Ogre::SceneManager *const scnMgr;
};

#endif
