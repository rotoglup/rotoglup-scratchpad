# use cmake -DCMAKE_BUILD_TYPE=type_of_build

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# This must be set _before_ project
#SET (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE INTERNAL "" FORCE) 

PROJECT(RTGU_libs)

OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
IF(MSVC)
	SET(DEFAULT_BOOST_DIRECTORY "C:/dev/dev_R&D/boost_1_35/include/boost-1_35")
	MESSAGE(STATUS "MSVC ${MSVC}")
ELSE()
	SET(DEFAULT_BOOST_DIRECTORY "/usr/local/boost_1_37_0")
	MESSAGE(STATUS "NOT MSVC ${MSVC}")
ENDIF()
SET(BOOST_DIRECTORY ${DEFAULT_BOOST_DIRECTORY} CACHE STRING "Boost root directory")
IF(NOT IS_DIRECTORY "${BOOST_DIRECTORY}")
	MESSAGE(STATUS "Boost root directory '${BOOST_DIRECTORY}' not found")
ENDIF()

# Set the LIB_TYPE variable to STATIC
SET (RTGU_LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
  SET (RTGU_LIB_TYPE SHARED)
ENDIF (BUILD_SHARED_LIBS)

string(TOLOWER "${CMAKE_BUILD_TYPE}" RTGU_BUILD_TYPE)
if (RTGU_BUILD_TYPE STREQUAL "debug")
  #add_definitions(-DDEBUG)
endif()
  
# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (eg MacroOutOfSourceBuild.cmake):
SET(CMAKE_MODULE_PATH ${RTGU_libs_SOURCE_DIR}/_cmake/modules ${CMAKE_MODULE_PATH})

###
#function(rtgu_library [arg1 [arg2 [arg3 ...]]])
#  COMMAND1(ARGS ...)
#  COMMAND2(ARGS ...)
#  ...
#endfunction(rtgu_library)
###

SET(RTGU_LIB_SUFFIX "")
IF (RTGU_BUILD_TYPE STREQUAL "debug")
  SET(RTGU_LIB_SUFFIX "-d")
ENDIF()

ADD_DEFINITIONS("-D _CRT_SECURE_NO_DEPRECATE -D _SECURE_SCL=0")

INCLUDE_DIRECTORIES( ${BOOST_DIRECTORY} ) 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../external ) 
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/.. ) 

ADD_SUBDIRECTORY(../external "${CMAKE_CURRENT_BINARY_DIR}/external")

INCLUDE_DIRECTORIES(${RTGU_libs_SOURCE_DIR}) 

ENABLE_TESTING()
#INCLUDE(Dart)
ADD_SUBDIRECTORY(tests)
