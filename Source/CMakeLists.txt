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

add_executable(Resourceful Resourceful.cpp)

install(TARGETS Resourceful DESTINATION bin)

