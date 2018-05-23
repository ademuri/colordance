project(libcolor LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES "*.cpp")

add_library(color ${SOURCES})
target_include_directories(color PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:.>)

add_executable(colortest test/colortest.cpp)

target_link_libraries(colortest color gmock_main gtest)

add_test(colortest colortest)
