#include <iostream>
#include "Resourceful/Resourceful.h"

int main(int argc, char* argv[]) {
  // Now relying on the library to locate things, so query it
  std::clog << "-- Basic Paths:" << std::endl;
  std::clog << "rsf::getLibraryDir() = " << rsf::getLibraryDir() << std::endl;
  std::clog << "rsf::getResourceRootDir() = " << rsf::getResourceRootDir() << std::endl;
  std::clog << "rsf::getPluginRootDir() = " << rsf::getPluginRootDir() << std::endl;
  
  std::clog << "\n-- Resources:" << std::endl;
  rsf::catResource("Resourceful.json", std::clog);
  rsf::catResource("DoesNotExist.json", std::clog);

  
  std::clog << "\n-- Plugins:" << std::endl;
  rsf::runPlugins(std::clog);

  return 0;
}
