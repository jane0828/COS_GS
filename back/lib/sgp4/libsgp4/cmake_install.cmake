# Install script for directory: /home/miman/sgs/back/lib/sgp4/libsgp4

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/miman/sgs/back/lib/sgp4/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsgp4s.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsgp4s.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsgp4s.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/miman/sgs/back/lib/sgp4/libsgp4/libsgp4s.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsgp4s.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsgp4s.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsgp4s.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libsgp4" TYPE FILE FILES
    "/home/miman/sgs/back/lib/sgp4/libsgp4/CoordGeodetic.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/CoordTopocentric.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/DateTime.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/DecayedException.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/Eci.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/Globals.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/Observer.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/OrbitalElements.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/SatelliteException.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/SGP4.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/SolarPosition.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/TimeSpan.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/TleException.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/Tle.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/Util.h"
    "/home/miman/sgs/back/lib/sgp4/libsgp4/Vector.h"
    )
endif()

