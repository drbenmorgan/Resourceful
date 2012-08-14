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

// Third Party
// - A

// This Project
#include "resourceful_binreloc.h"

int main() {
  std::cout << "[Resourceful::Info] Startup!" << std::endl;

  // - Try use of binreloc directly....
  // Initialization
  BrInitError err;
  if (!br_init(&err)) {
    std::cerr << "[Resourceful::Error] Failed to locate self (" 
              << err
              << ")" << std::endl;
   return 1;
  } 

  // Basic exe paths (remember to free!)
  char* exe_path(0);
  exe_path = br_find_exe("");
  std::cout << "[Resourceful::Info] Located self at " << exe_path << std::endl;
  free(exe_path);

  char* exe_dir(0);
  exe_dir = br_find_exe_dir("");
  std::cout << "[Resourceful::Info] Self in directory " << exe_dir << std::endl;
  free(exe_dir);

  return 0;
}





