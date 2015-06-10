// ResourcefulBasic - argv[0] not always reliable
//
// The actually file given by argv[0] might be a soft or hard link
// Though a softlink chain can be followed, it's messy.
// Hardlinks are more difficult because though we can get a link
// count, and even locate all inodes, following these all to filesystem
// paths and then determining the correct one is very hard.
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

#include <sys/stat.h>
#include <unistd.h>

// Third Party
// - A

int main(int argc, char* argv[]) {
  std::cout << "[RB::Info] Startup!" << std::endl;
  std::cout << "[RB::Info] Call path : " << argv[0] << std::endl;

  struct stat thisApplication;
  int statRes = stat(argv[0], &thisApplication);

  if (statRes == -1) {
    std::cout << "[RBInfo::Error] could not stat self" << std::endl;
    return errno;
  }

  // Now, stat follows a symlink but only one level
  std::cout << "[RB::Info] " << argv[0] << " is a ";

  if ( (thisApplication.st_mode & S_IFMT) == S_IFREG ) {
    std::cout << "regular file" << std::endl;
  }
  if ( (thisApplication.st_mode & S_IFMT) == S_IFLNK ) {
    std::cout << "symbolic link to " << std::endl;
    // Try and resolve
    char buf[1024];
    ssize_t len;

    if ((len = readlink(argv[0], buf, sizeof(buf)-1)) != -1) {
      buf[len] = '\0';
      std::cout << buf << std::endl;
    }
  }
  else {
    std::cout << "something else" << std::endl;
  }

  // Can get hardlink count, but can't easily locate original.

  return 0;
}

