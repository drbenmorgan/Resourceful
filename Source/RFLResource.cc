// RFLResource.cc - Implementation of the RFLResource
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

// - Ourselves
#include "RFLResource.h"

// Standard Library

// Third Party
// - Boost
#include <boost/filesystem.hpp>

// This Project
#include "resourceful_config.h"
#include "resourceful_binreloc.h"

namespace RFLResource {
bool initResources() {
  BrInitError err;
  return br_init(&err);
}

std::string getApplicationPath() {
  char* exePath(0);
  exePath = br_find_exe_dir("");
  boost::filesystem::path sExePath(exePath);
  free(exePath);
  boost::filesystem::path cExePath = boost::filesystem::canonical(sExePath);
  return cExePath.string();
}

std::string getResourcePath(const std::string& internalPath) {
  boost::filesystem::path resPath(getApplicationPath());
  resPath /= getAppToResourcePath();
  resPath /= internalPath;
  boost::filesystem::path cResPath;
  try {
    cResPath = boost::filesystem::canonical(resPath);
  } catch (std::exception& e) {
    return std::string();
  }
  return cResPath.string();
}
} // namespace RFLResource

