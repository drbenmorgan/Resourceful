#include <iostream>
#include "RSFSimple_binreloc.h"

int main(int argc, char* argv[]) {
  // Initialize binreloc system and error check
  BrInitError err;
  int brOK = br_init(&err);
  if (brOK != 1) {
    std::cerr << "rsf_simple: Binreloc failed to init, error code : " << err << std::endl;
    return 1;
  }

  // Find ourself...
  char* myPath = br_find_exe("");
  std::clog << "rsf_simple : br_find_exe = " << myPath << std::endl;
  free(myPath);

  // What the system called us as...
  std::clog << "rsf_simple : argv[0] = " << argv[0] << std::endl;

  // Find the directory we are in
  char* myDir = br_find_exe_dir("");
  std::clog << "rsf_simple : br_find_exe_dir = " << myDir << std::endl;
  free(myDir);

  return 0;
}
