#ifndef __LOCAL_LIGHT_CONTROLLER_HPP__
#define __LOCAL_LIGHT_CONTROLLER_HPP__

#include <chrono>
#include "../color/Color.hpp"
#include "../controller/LightController.hpp"

/**
 * Used to control DMX lights over an FTDI USB to RS485 adapter.
 */
class LocalLightController : public LightController {
 public:
  LocalLightController();

  void Set(const uint16_t lightId, HSV hsv) override;
  uint16_t GetMs() override;

  /**
   * Writes out all DMX data. Should be called once per loop. Moderately
   * expensive (takes ~25ms to write 512 channels).
   */
  void WriteDmx();

 private:
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

  // TODO: size this once we know the actual number of physical lights.
  // Should be <num lights> * 8 + 1.
  static const int kSerialBufSize = 153;
  unsigned char serialBuf[kSerialBufSize];

  /**
   * Map from light id (i.e. DMX starting channel) to the current color. When we
   * send the DMX data, we have to send data to all of the lights, so we need to
   * keep track of the state.
   */
  std::map<const uint16_t, RGB> idToColorMap;

  /**
   * FTDI context used for sending DMX signals.
   */
  struct ftdi_context *ftdi;
};
#endif
