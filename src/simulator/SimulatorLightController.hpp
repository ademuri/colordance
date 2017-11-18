#ifndef __SIMULATOR_LIGHT_CONTROLLER_HPP__
#define __SIMULATOR_LIGHT_CONTROLLER_HPP__

#include "../color/Color.hpp"
#include "../controller/LightController.hpp"
#include <OgreLight.h>
#include <map>

/*
 * Interface class for controlling the output lights. Eventually, there will be
 * a subclass of this that uses DMX to control real lights.
 */
class SimulatorLightController : public LightController {
public:
  SimulatorLightController(Ogre::Light *left, Ogre::Light *center,
                           Ogre::Light *right);

  void Set(const Lights light, HSV hsv) override;

private:
  std::map<const Lights, Ogre::Light *> lightMap;
};

#endif