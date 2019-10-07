#include <iostream>
#include "Resourceful/Resourceful.h"

int main(int argc, char* argv[]) {
  // Now relying on the library to locate things, so query it
  std::clog << "rsf_library : rsf::getLibraryDir() = " << rsf::getLibraryDir() << std::endl;
  std::clog << "rsf_library : rsf::getResourceRootDir() = " << rsf::getResourceRootDir() << std::endl;
  std::clog << "rsf_library : rsf::getPluginRootDir() = " << rsf::getPluginRootDir() << std::endl;
  return 0;
}
