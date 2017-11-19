#ifndef __SIMULATOR_LIGHT_CONTROLLER_HPP__
#define __SIMULATOR_LIGHT_CONTROLLER_HPP__

#include "../color/Color.hpp"
#include "../controller/LightController.hpp"
#include <OgreLight.h>
#include <chrono>
#include <map>

/*
 * Interface class for controlling the output lights. Eventually, there will be
 * a subclass of this that uses DMX to control real lights.
 */
class SimulatorLightController : public LightController {
public:
  SimulatorLightController(Ogre::Light *left, Ogre::Light *center,
                           Ogre::Light *right, Ogre::Light *top,
                           Ogre::Light *bottom);

  void Set(const Lights light, HSV hsv) override;

  uint16_t GetMs() override;

private:
  std::map<const Lights, Ogre::Light *> lightMap;

  const std::chrono::steady_clock::time_point startTime;
};

#endif
