#include "LocalLightController.hpp"
#include <cstdio>
#include <iostream>

LocalLightController::LocalLightController() {
  lightIds = {{1, 9, 17}};
  serialPort = new boost::asio::serial_port(io_service_, "/dev/ttyACM0");

  std::vector<std::vector<uint16_t>>::iterator rowIt;
  std::vector<uint16_t>::iterator colIt;
  // The lights have a global brightness channel. Initialize it to 255 (full
  // on).
  for (rowIt = lightIds.begin(); rowIt != lightIds.end(); rowIt++) {
    for (colIt = (*rowIt).begin(); colIt != (*rowIt).end(); colIt++) {
      std::ostringstream stream;
      stream << *colIt + kMasterOffset << "c255w";
      if (!serialPort->write_some(boost::asio::buffer(stream.str()))) {
        printf("Error writing initialization DMX command to serial port!\n");
      }
    }
  }

  numRows = 1;
  numCols = 3;
}

void LocalLightController::Set(const uint16_t lightId, HSV hsv) {
  RGB rgb = Color::toRGB(hsv);
  std::ostringstream stream;
  // Note: we have to explicitly use to_string here, or else if the values are
  // 0, they are omitted entirely.
  stream << lightId + kRedOffset << "c" << std::to_string(rgb.r) << "w";
  stream << lightId + kGreenOffset << "c" << std::to_string(rgb.g) << "w";
  stream << lightId + kBlueOffset << "c" << std::to_string(rgb.b) << "w";
  if (!serialPort->write_some(boost::asio::buffer(stream.str()))) {
    printf("Error writing DMX command to serial port!\n");
  }
}

uint16_t LocalLightController::GetMs() {
  const std::chrono::steady_clock::time_point endTime =
      std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                               startTime)
      .count();
}
