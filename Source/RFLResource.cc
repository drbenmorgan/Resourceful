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

//! Local private namespace for storing paths
namespace {
struct SearchPathImpl {
  //! Hold paths, NB, not MT safe
  typedef std::list<boost::filesystem::path> SearchPath;

  void prependSearchPath(const std::string& path) {
    // NB, don't check existance, expand ~ or env vars!
    // Also no policy on how to handle relative paths!
    dirs_.push_front(boost::filesystem::path(path));
  }

  //! Return first fully resolved path in search path
  std::string findFirstInPath(const std::string& queriedPath) {
    boost::filesystem::path localPath(queriedPath);
    // An absolute path is already fully resolved, as is a search with no paths
    if (localPath.is_absolute() || dirs_.empty()) {
      return queriedPath;
    }

    auto resolvedPath = std::find_if(dirs_.begin(),
                                     dirs_.end(),
                                     [&localPath](const boost::filesystem::path& p) {
                                     return boost::filesystem::exists(p / localPath) ? true : false;
                                     }
                                     );

    if (resolvedPath == dirs_.end()) {
      // Could not find an existing path in the search dirs, return null?
      return std::string();
    }

    return ((*resolvedPath) / localPath).string();
  }

 private:
  SearchPath dirs_;
};

static SearchPathImpl RFLResourcePath;
}

namespace RFLResource {
bool initResources() {
  BrInitError err;
  bool resourcesInitd = br_init(&err);
  if(resourcesInitd) {
    // Create the base resource search paths
    boost::filesystem::path baseResourcePath(getApplicationPath());
    baseResourcePath /= getAppToResourcePath();
    boost::filesystem::path canonicalResourcePath = boost::filesystem::canonical(baseResourcePath);
    RFLResourcePath.prependSearchPath(canonicalResourcePath.string());
  }

  return resourcesInitd;
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
  boost::filesystem::path foundPath = RFLResourcePath.findFirstInPath(internalPath);

  boost::filesystem::path cResPath;
  try {
    cResPath = boost::filesystem::canonical(foundPath);
  } catch (std::exception& e) {
    return std::string();
  }
  return cResPath.string();
}

void prependToResourcePath(const std::string& path) {
  RFLResourcePath.prependSearchPath(path);
}
} // namespace RFLResource

