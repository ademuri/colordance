include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if(${Boost_FOUND})
  add_definitions(-DUSE_BOOST=1)
  message(STATUS "Using Boost")
endif()

add_executable(Local Local.cpp LocalLightController.cpp LocalParamController.cpp)
target_link_libraries(Local ${Boost_LIBRARIES} controller color)

