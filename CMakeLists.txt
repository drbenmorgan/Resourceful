# - Top level build script for Resourceful
#
#-----------------------------------------------------------------------
# Copyright (c) 2012-19, Ben Morgan <Ben.Morgan@warwick.ac.uk>
# Copyright 2012-19 University of Warwick
#
# Distributed under the OSI-approved BSD 3-Clause License (the "License");
# see accompanying file License.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#-----------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Resourceful VERSION 0.1.0)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

#----------------------------------------------------------------------
# Define the project configuration
# Allow configuration of the CMAKE_CXX_STANDARD variable
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ Standard to compile against")

# - Use Boost to provide filesystem in lieu of C++17 fs
#   Also an example of relocation with a third-party dependency
find_package(Boost 1.60 REQUIRED filesystem)

# - Binreloc Generator
include(binreloc/GenerateBinreloc)

# - Install paths: a standard *NIX program, libraries, and resources
include(GNUInstallDirs)

# Add a path for plugins (immutable)
set(CMAKE_INSTALL_RSFPLUGINDIR "${CMAKE_INSTALL_LIBDIR}/rsf")

# Add a path for CMake config files (immutable)
set(CMAKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake")
set(CMAKE_INSTALL_FULL_CMAKEDIR "${CMAKE_INSTALL_FULL_LIBDIR}/cmake")

# Add a path for pkgconf files (immutable)
set(CMAKE_INSTALL_PKGCONFIGDIR "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
set(CMAKE_INSTALL_FULL_PKGCONFIGDIR "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")

# - Calculate relative paths between known absolute install location of binaries/library
#   and resource/plugin dirs
file(RELATIVE_PATH PROJECT_BINDIR_TO_LIBDIR "${CMAKE_INSTALL_FULL_BINDIR}" "${CMAKE_INSTALL_FULL_LIBDIR}")
file(RELATIVE_PATH PROJECT_LIBDIR_TO_RESOURCEDIR "${CMAKE_INSTALL_FULL_LIBDIR}" "${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}")
set(PROJECT_LIBDIR_TO_PLUGINDIR "rsf")

#-----------------------------------------------------------------------
# One gotcha with self-location is that the paths must also exist in the
# tree
# Bit of a grey area between design and self-location : e.g., could use
# an environment variable to override for testing.
#
# All we do here is create an output tree for build products that matches
# the runtime install paths.
#.rst
# In all cases, build products are stored in a directory tree rooted
# in a directory named ``BuildProducts`` under the ``PROJECT_BINARY_DIRECTORY``.
#
set(BASE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts")

#.rst:
# For single mode build generators (make, ninja), the following
#  hierarchy is used:
#
#  .. code-block:: console
#
#    +- <PROJECT_BINARY_DIR>/
#       +- BuildProducts/
#          +- <CMAKE_INSTALL_BINDIR>/
#             +- ... "runtime" targets ...
#          +- <CMAKE_INSTALL_LIBDIR>/
#             +- ... "library" and "archive" targets ...
#
#  For multimode build generators (Xcode, Visual Studio), each mode
#  is separated using the hierarchy
#
#  .. code-block:: console
#
#    +- <PROJECT_BINARY_DIR>
#       +- BuildProducts/
#          +- <CONFIG>/
#             +- <CMAKE_INSTALL_BINDIR>/
#                +- ... "runtime" targets ...
#             +- <CMAKE_INSTALL_LIBDIR>/
#                +- ... "library" and "archive" targets ...
#          +- ...
#
#  where ``<CONFIG>`` is repeated for each build configuration listed in
#  :cmake:variable:`CMAKE_CONFIGURATION_TYPES <cmake:variable:CMAKE_CONFIGURATION_TYPES>`, e.g. Release, Debug, RelWithDebInfo etc.

if(NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_BINDIR}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")

  # configure_file is a bit of a messy way to create the build tree resources
  # More elegant and robust solutions likely!
  configure_file(resources/Resourceful.json "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_DATAROOTDIR}/Resourceful/Resourceful.json" COPYONLY)
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/$<CONFIG>/${CMAKE_INSTALL_BINDIR}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/$<CONFIG>/${CMAKE_INSTALL_LIBDIR}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/$<CONFIG>/${CMAKE_INSTALL_LIBDIR}")
  
  # CMake awkwardness - configure_file doesn't support genexes
  configure_file(resources/Resourceful.json "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_DATAROOTDIR}/Resourceful/Resourceful.json" COPYONLY)
  file(GENERATE
    OUTPUT "${BASE_OUTPUT_DIRECTORY}/$<CONFIG>/${CMAKE_INSTALL_DATAROOTDIR}/Resourceful/Resourceful.json"
    INPUT "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_DATAROOTDIR}/Resourceful/Resourceful.json")

endif()


#-----------------------------------------------------------------------
# Configure testing
#
include(CTest)

#----------------------------------------------------------------------
# Build subdirectories
#
add_subdirectory(Resourceful)
add_subdirectory(programs)

#----------------------------------------------------------------------
# Install resources
#
install(DIRECTORY resources/ DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/Resourceful")

#-----------------------------------------------------------------------
# Configure/install CMake/pkgconfig helpers
# Note that it's possible to make two versions of these files
# - The familiar ones we install ("install tree")
# - Versions that turn the build directory ("build tree") into a usable
#   albeit not relocatable package.

# For pkg-config, need relative path from pkgconfig dir to prefix
file(RELATIVE_PATH PROJECT_PCFILEDIR_TO_PREFIX "${CMAKE_INSTALL_FULL_PKGCONFIGDIR}" "${CMAKE_INSTALL_PREFIX}")
configure_file(cmake/Resourceful.pc.in Resourceful.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Resourceful.pc DESTINATION "${CMAKE_INSTALL_PKGCONFIGDIR}")

# CMake handles the paths for us...
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(cmake/ResourcefulConfig.cmake.in ResourcefulConfig.cmake
  INSTALL_DESTINATION "${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME}"
  # Strictly speaking we don't need PATH_VARS as we rely on targets
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ResourcefulConfigVersion.cmake ${CMAKE_CURRENT_BINARY_DIR}/ResourcefulConfig.cmake
  DESTINATION "${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME}")

# Now the most important bit - the exports file that will define the imported target(s)
# that clients can link to
install(EXPORT ${PROJECT_NAME}Targets
  DESTINATION "${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME}"
  # Namespace is useful because it leads to strong guarantees on
  # existence and location of what we end up linking to
  NAMESPACE ${PROJECT_NAME}::)

