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
if(APPLE AND UNIX)
  set(RESOURCEFUL_RESOURCEPATH "../Resources")
else()
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

