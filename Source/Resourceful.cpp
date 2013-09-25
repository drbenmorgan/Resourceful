// Resourceful - access application resource files using C++
//
//----------------------------------------------------------------------
// Copyright (c) 2012, Ben Morgan <bmorgan.warwick@gmail.com>
// Copyright (c) 2012, University of Warwick
//
// Distributed under the OSI-approved BSD 3-Clause License (the "License");
// see accompanying file License.txt for details.
//
// This software is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the License for more information.
//----------------------------------------------------------------------

// Standard Library
#include <iostream>
#include <fstream>

// Third Party
// - A

// This Project
#include "RFLResource.h"

int main() {
  std::cout << "[Resourceful::Info] Startup!" << std::endl;
  std::cout << "[Resourceful::Info] initResource()" << std::endl;
  if (!RFLResource::initResources()) {
    std::cerr << "[Resourceful::Error] Could not initialize resources" << std::endl;
    return 1;
  }

  // - Log the application path
  std::string appPath = RFLResource::getApplicationPath();
  std::cout << "[Resourceful::Info] getApplicationPath() = " 
            << RFLResource::getApplicationPath()
            << std::endl;

  // - Log the resource path
  std::cout << "[Resourceful::Info] getResourcePath(foo) = "
            << RFLResource::getResourcePath("startupScript")
            << std::endl;
  
  std::string reqResourceFile = RFLResource::getResourcePath("startupScript");
  if (reqResourceFile.empty()) {
    std::cerr << "[Resourceful::error] Invalid resource" << std::endl;
    return 1;
  }
  
  std::ifstream startupScriptStream;
  startupScriptStream.open(reqResourceFile.c_str());
  if(!startupScriptStream) {
    std::cerr << "[Resourceful::error] Failed to open resource" << std::endl;
    return 1;
  } else {
    std::cout << "[Resourceful::info] Opened resource" << std::endl;
    std::string sLine;
    while (std::getline(startupScriptStream,sLine)) {
      std::cout << sLine << std::endl;
    }
  }

  return 0;
}





