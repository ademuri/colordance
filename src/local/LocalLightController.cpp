#include "LocalLightController.hpp"
#include <cstdio>
#include <iostream>

#include <ftdi.h>
#include <unistd.h>

LocalLightController::LocalLightController()
    : startTime(std::chrono::steady_clock::now()) {
  lightIds = {{1, 9, 17}};

  std::vector<std::vector<uint16_t>>::iterator rowIt;
  std::vector<uint16_t>::iterator colIt;
  for (rowIt = lightIds.begin(); rowIt != lightIds.end(); rowIt++) {
    for (colIt = (*rowIt).begin(); colIt != (*rowIt).end(); colIt++) {
      idToColorMap[*colIt] = {0, 0, 0};
    }
  }

  numRows = 1;
  numCols = 3;

  // Initialize the FTDI device.
  if ((ftdi = ftdi_new()) == 0) {
    printf("ftdi_new failed\n");
    return;
  }

  int ret;
  if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) != 0) {
    printf("usb_open failed: %d\n", ret);
  }

  if ((ret = ftdi_set_baudrate(ftdi, 250000))) {
    printf("set baudrate failed: %d\n", ret);
  }

  if ((ret = ftdi_set_line_property2(ftdi, BITS_8, STOP_BIT_2, NONE,
                                     BREAK_OFF))) {
    printf("set line property failed: %d\n", ret);
  }

  if ((ret = ftdi_setflowctrl(ftdi, SIO_DISABLE_FLOW_CTRL))) {
    printf("set flow control failed: %d\n", ret);
  }

  for (int i = 0; i < kSerialBufSize; i++) {
    serialBuf[i] = 0;
  }
}

void LocalLightController::Set(const uint16_t lightId, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  idToColorMap[lightId] = rgb;
}

uint16_t LocalLightController::GetMs() {
  const std::chrono::steady_clock::time_point endTime =
      std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                               startTime)
      .count();
}

void LocalLightController::WriteDmx() {
  std::vector<std::vector<uint16_t>>::iterator rowIt;
  std::vector<uint16_t>::iterator colIt;
  for (rowIt = lightIds.begin(); rowIt != lightIds.end(); rowIt++) {
    for (colIt = (*rowIt).begin(); colIt != (*rowIt).end(); colIt++) {
      serialBuf[*colIt + kMasterOffset] = 255;
      serialBuf[*colIt + kRedOffset] = idToColorMap[*colIt].r;
      serialBuf[*colIt + kGreenOffset] = idToColorMap[*colIt].g;
      serialBuf[*colIt + kBlueOffset] = idToColorMap[*colIt].b;
    }
  }

  int ret;

  // A DMX packet starts with a break of at least 88us. Note: on my desktop, the
  // break actually lasts for ~350ms with usleep(100). That's fine, as long as
  // the break is >88us.
  if ((ret =
           ftdi_set_line_property2(ftdi, BITS_8, STOP_BIT_2, NONE, BREAK_ON))) {
    printf("set line property failed: %d\n", ret);
  }
  usleep(100);
  if ((ret = ftdi_set_line_property2(ftdi, BITS_8, STOP_BIT_2, NONE,
                                     BREAK_OFF))) {
    printf("set line property failed: %d\n", ret);
  }

  if ((ret = ftdi_write_data(ftdi, serialBuf, kSerialBufSize)) < 0) {
    printf("write data failed: %d\n", ret);
  }
}
