// RFLResource.h - Resourceful interface for accessing app resources
//
// Copyright (c) 2013 by Ben Morgan <bmorgan.warwick@gmail.com> 
// Copyright (c) 2013 by The University of Warwick
//
// Distributed under the OSI-approved BSD 3-Clause License (the "License");
// see accompanying file License.txt for details.
//
// This software is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the License for more information.
#ifndef RFLRESOURCE_HH
#define RFLRESOURCE_HH
// Standard Library
#include <string>

// Third Party
// - A

// This Project

namespace RFLResource {
//! Initialize the application resources, returning false if they cannot be
bool initResources();

//! Return the path to the application directory
std::string getApplicationPath();

//! Return path to named resource
std::string getResourcePath(const std::string& internalPath);
}
#endif // RFLRESOURCE_HH



