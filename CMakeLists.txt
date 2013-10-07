# - Top level build script for Resourceful
#
#-----------------------------------------------------------------------
# Copyright (c) 2012, Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick
#
# Distributed under the OSI-approved BSD 3-Clause License (the "License");
# see accompanying file License.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#-----------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Resourceful)

#----------------------------------------------------------------------
# Define the project configuration
#
# - Versioning
set(Resourceful_VERSION_MAJOR 0)
set(Resourceful_VERSION_MINOR 1)
set(Resourceful_VERSION_PATCH 0)

# - Build as .app on Mac, standard tool on other platforms
# Trick here is that we want to recreate the relative path arrangement
# of the install environment in the build so that running the program
# in the build directory works.
# There are different hierarchies for .app bundles and a "unix tool"
# but see how CMake is bundled for a counter example of using a
# Unix layout inside a .app - that may be the most portable way
if(APPLE AND UNIX)
  set(RESOURCEFUL_RESOURCEPATH "../Resources")
  set(RESOURCEFUL_PACKAGE_LOCATION "Resources")
else()
  include(GNUInstallDirs)
  set(CMAKE_INSTALL_RESOURCEDIR "${CMAKE_INSTALL_DATAROOTDIR}/Resourceful")
  set(RESOURCEFUL_PACKAGE_LOCATION "../${CMAKE_INSTALL_RESOURCEDIR}")

  if(IS_ABSOLUTE "${CMAKE_INSTALL_RESOURCEDIR}")
    set(CMAKE_INSTALL_FULL_RESOURCEDIR "${CMAKE_INSTALL_RESOURCEDIR}")
  else()
    set(CMAKE_INSTALL_FULL_RESOURCEDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_RESOURCEDIR}")
  endif()
 
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts/${CMAKE_INSTALL_BINDIR}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts/${CMAKE_INSTALL_LIBDIR}")

  file(RELATIVE_PATH RESOURCEFUL_RESOURCEPATH "${CMAKE_INSTALL_FULL_BINDIR}" "${CMAKE_INSTALL_FULL_RESOURCEDIR}")
endif()

#----------------------------------------------------------------------
# Build subdirectories
#
add_subdirectory(Source)

#-----------------------------------------------------------------------
# Add testing with dashboard submission
#
enable_testing()
include(CTest)
add_subdirectory(Tests)

