project(libcontroller LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES "*.cpp")

add_library(controller ${SOURCES})
target_include_directories(controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:.>)

file(GLOB TEST_SOURCES "test/*.cpp")

add_executable(controllertest ${TEST_SOURCES})

target_link_libraries(controllertest controller gmock_main gtest)

add_test(controllertest controllertest)
