include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(libftdi REQUIRED)
include_directories(SYSTEM ${LIBFTDI_INCLUDE_DIRS})

add_executable(Local Local.cpp LocalLightController.cpp LocalParamController.cpp)
target_link_libraries(Local ${Boost_LIBRARIES} ${LIBFTDI_LIBRARIES} controller color)

