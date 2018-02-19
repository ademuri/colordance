#ifndef __LOCAL_LIGHT_CONTROLLER_HPP__
#define __LOCAL_LIGHT_CONTROLLER_HPP__

#include <chrono>
#include "../color/Color.hpp"
#include "../controller/LightController.hpp"

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

/**
 * Used to control DMX lights by talking to an Arduino over its serial port.
 * The Arduino should use
 * https://github.com/PaulStoffregen/DmxSimple/blob/master/examples/SerialToDmx/SerialToDmx.pde
 */
class LocalLightController : public LightController {
 public:
  LocalLightController();

  void Set(const uint16_t lightId, HSV hsv) override;
  uint16_t GetMs() override;

 private:
  boost::asio::io_service io_service_;
  boost::asio::serial_port *serialPort = nullptr;

  /** Maps from public light id to DMX starting channel. */
  std::map<const uint16_t, const uint16_t> lightIdMap;

  const std::chrono::steady_clock::time_point startTime;

  /**
   * DMX works by assigning each device an id (the starting channel). The
   * lights used have 8 channels, so a device with id of 1 will use channels
   * 1-8. Channel 4 is the master brightness, and following three channels are
   * red, green, and blue.
   */
  const uint16_t kMasterOffset = 3;
  const uint16_t kRedOffset = 4;
  const uint16_t kGreenOffset = 5;
  const uint16_t kBlueOffset = 6;
};
#endif
