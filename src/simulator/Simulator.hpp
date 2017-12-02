#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreLight.h>
#include <OgreRoot.h>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "../controller/Effect.hpp"
#include "../controller/ParamController.hpp"
#include "SimulatorLightController.hpp"

#ifdef USE_BOOST
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#endif

enum class ControlKeys {
  kUp,
  kDown,
};

class Simulator : public OgreBites::ApplicationContext,
                  public OgreBites::InputListener {
 public:
  Simulator();

  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  bool keyReleased(const OgreBites::KeyboardEvent &evt) override;
  void setup() override;
  bool frameEnded(const Ogre::FrameEvent &evt) override;

 private:
  SimulatorLightController *controller;
  ParamController *paramController;
  Effect *effect;

  // Keeps track of whether keys are pressed down
  std::map<ControlKeys, bool> keyDownMap;

  // Counter to only check the arrow key state every so often, so that when
  // held down they don't change params too fast. This is a janky, but simple
  // way of doing it, because OgreBites doesn't appear to do this for you.
  uint16_t keyDownDebounce = 0;

  // Used to allow manually typing in the number value of tempo. Accumulates
  // numbers when they're pressed, and is cleared upon pressing ENTER.
  std::string numberInputBuffer;

  // The list of params that are adjustable with the arrow keys
  std::vector<Params> adjustableParams = {
      Params::kTempo, Params::kWidth, Params::kPan, Params::kTilt,
  };

  // The list of params that are adjustable with the serial port.
  std::vector<Params> serialParams = {
      Params::kHue0,  // TODO: val
      Params::kWidth, Params::kPan,
      Params::kHue0,  // TODO: height
      Params::kHue0,  // TODO: sat
      Params::kTilt,
  };

  std::vector<Params> chooseLightParams = {
      Params::kWidth, Params::kPan, Params::kTilt,
  };

  // Which param in the above vector is currently being adjusted
  int16_t currentParamIndex = 0;

// Functionality for reading param values from the serial port. This is for
// reading params from a Teensy-based control surface, which sends the param
// values over the serial port. This is optional, and requires Boost.
#ifdef USE_BOOST
  void read_handler(const boost::system::error_code &error,
                    std::size_t bytes_transferred);

  boost::asio::io_service io_service_;
  boost::asio::serial_port *serialPort = nullptr;
  static const int kSerialBufSize = 200;
  char serialBuf[kSerialBufSize];
#endif
};

#endif
