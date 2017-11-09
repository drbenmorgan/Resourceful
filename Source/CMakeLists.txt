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
# "Dumb" executable for studying by hand self-location using only c/system
add_executable(ResourcefulBasic ResourcefulBasic.cpp)

#-----------------------------------------------------------------------
# Boost usage - filesystem used to canonize paths, but this is
# for pure cozmetix. Its usage is private so can be swapped out
# if you want to use other methods for paths or just don't care
#
find_package(Boost 1.60 REQUIRED filesystem)

#-----------------------------------------------------------------------
# Want to use binreloc in several places, so allow it to be configured
# - assume presence of binreloc.h.in and binreloc.c.in
# - Can configure file names as PREFIX_binreloc.X
# - Auto configure mangling of symbol names using MANGLE_BINRELOC variable
set(BINRELOC_CONFIG_PREFIX "resourceful_")
string(RANDOM LENGTH 32 BINRELOC_CONFIG_MANGLE)
set(BINRELOC_CONFIG_MANGLE "BCM_${BINRELOC_CONFIG_MANGLE}")
configure_file(binreloc.h.in ${BINRELOC_CONFIG_PREFIX}binreloc.h @ONLY)
configure_file(binreloc.c.in ${BINRELOC_CONFIG_PREFIX}binreloc.c @ONLY)

# "Trick" Use an INTERFACE target to collate sources that must be consumed by clients
# Just need to link privatly to pull sources in at level of that target only
# PRIVATE link needed otherwise clients of target also compile the sources!!
# NB: All binreloc names will be mangled identically, so cannot be used
#     in multiple targets that end up linked to each other!
#
# NB: Use this here over an OBJECT lib both to test the target_sources concept
#     and also because we're going to compile both SHARED and STATIC objects.
#     For these we need to compile two object libs (PIC and non-PIC) so
#     use of an INTERFACE lib allows that to be done automatically by
#     consuming client of binreloc. As we only have a couple of sources
#     for binreloc and a couple of clients the recompilation cost is minor.
#
# See
#
# https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/
#
# For a very useful guide to target_sources

add_library(${BINRELOC_CONFIG_PREFIX}binreloc INTERFACE)
target_sources(${BINRELOC_CONFIG_PREFIX}binreloc INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${BINRELOC_CONFIG_PREFIX}binreloc.h ${CMAKE_CURRENT_BINARY_DIR}/${BINRELOC_CONFIG_PREFIX}binreloc.c)
target_compile_definitions(${BINRELOC_CONFIG_PREFIX}binreloc INTERFACE ENABLE_BINRELOC)
target_include_directories(${BINRELOC_CONFIG_PREFIX}binreloc INTERFACE ${CMAKE_CURRENT_BINARY_DIR})


#-----------------------------------------------------------------------
# Define Sources/Resources
#
configure_file(resourceful_config.h.in resourceful_config.h @ONLY)
set(RESOURCEFUL_LIB_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/resourceful_config.h
  RFLResource.h
  RFLResource.cc
  )
set(RESOURCEFUL_RESOURCES startupScript)

#-----------------------------------------------------------------------
# Single application Target
#
if(BUILD_APP_BUNDLE)
  set_property(SOURCE ${RESOURCEFUL_RESOURCES}
    PROPERTY
      MACOSX_PACKAGE_LOCATION "${RESOURCEFUL_PACKAGE_LOCATION}"
    )
  add_executable(Resourceful MACOSX_BUNDLE Resourceful.cpp ${RESOURCEFUL_LIB_SOURCES} ${RESOURCEFUL_RESOURCES})
else()
  foreach(_res ${RESOURCEFUL_RESOURCES})
    configure_file(${_res} ${PROJECT_BINARY_DIR}/BuildProducts/${CMAKE_INSTALL_RESOURCEDIR}/${_res} COPYONLY)
  endforeach()
  add_executable(Resourceful Resourceful.cpp ${RESOURCEFUL_LIB_SOURCES} ${RESOURCEFUL_RESOURCES})
endif()

target_link_libraries(Resourceful PUBLIC Boost::filesystem PRIVATE resourceful_binreloc)


#-----------------------------------------------------------------------
# Application as client of shared library
#
add_library(ResourcefulInterface SHARED ${RESOURCEFUL_LIB_SOURCES})
target_link_libraries(ResourcefulInterface PUBLIC Boost::filesystem PRIVATE resourceful_binreloc)

add_library(ResourcefulInterface-static STATIC ${RESOURCEFUL_LIB_SOURCES})
target_link_libraries(ResourcefulInterface-static PUBLIC Boost::filesystem PRIVATE resourceful_binreloc)

add_executable(ResourcefulClient Resourceful.cpp)
target_link_libraries(ResourcefulClient ResourcefulInterface)

add_executable(ResourcefulClient-static Resourceful.cpp)
target_link_libraries(ResourcefulClient-static ResourcefulInterface-static)


install(TARGETS ResourcefulInterface ResourcefulClient
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )

