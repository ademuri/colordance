project(libcontroller LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES "*.cpp")

add_library(controller ${SOURCES})
target_include_directories(controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:.>)
