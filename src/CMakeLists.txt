cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0028 NEW)
project(colordance LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${colordance_SOURCE_DIR}/cmake")

# Set an output directory for our binaries
set(BIN_DIR ${colordance_SOURCE_DIR}/bin)

# Set appropriate warning levels for clang, gcc & msvc.
# Also set debug/optimization flags depending on the build type.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Ogre3d
# specify which version you need
find_package(OGRE 1.10 REQUIRED)

# the search paths
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})

# copy essential config files next to our binary where OGRE autodiscovers them
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg
     DESTINATION ${CMAKE_BINARY_DIR})

file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(simulator)
