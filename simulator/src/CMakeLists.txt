# Ogre3d
# specify which version you need
find_package(OGRE 1.10 REQUIRED)

# the search paths
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})

# copy essential config files next to our binary where OGRE autodiscovers them
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg
     DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Simulator Simulator.cpp Simulator.hpp)
target_link_libraries(Simulator ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${Boost_LIBRARIES} ${FREETYPE_LIBRARIES})
