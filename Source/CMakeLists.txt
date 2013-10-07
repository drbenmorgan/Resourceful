# - Build and install Resourceful binaries
#
#-----------------------------------------------------------------------
# Copyright 2012 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick
#
# Distributed under the OSI-approved 3-Clause License (the "License");
# see the accompanying file "License.txt" for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Boost usage - filesystem used to canonize paths, but this is
# for pure cozmetix. Its usage is private so can be swapped out
# if you want to use other methods for paths or just don't care
#
find_package(Boost 1.43 REQUIRED filesystem system)

add_definitions(-DENABLE_BINRELOC)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})

#-----------------------------------------------------------------------
# Define Sources/Resources
#
configure_file(resourceful_config.h.in resourceful_config.h @ONLY)
set(RESOURCEFUL_SOURCES
  Resourceful.cpp
  resourceful_binreloc.h
  resourceful_binreloc.c
  resourceful_config.h
  RFLResource.h
  RFLResource.cc
  )
set(RESOURCEFUL_RESOURCES startupScript)

set_property(SOURCE ${RESOURCEFUL_RESOURCES} 
  PROPERTY 
    MACOSX_PACKAGE_LOCATION "${RESOURCEFUL_PACKAGE_LOCATION}"
  )

#-----------------------------------------------------------------------
# Target
#
add_executable(Resourceful MACOSX_BUNDLE ${RESOURCEFUL_SOURCES} ${RESOURCEFUL_RESOURCES})
target_link_libraries(Resourceful ${Boost_LIBRARIES})

