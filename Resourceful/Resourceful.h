#ifndef RESOURCEFUL_HH
#define RESOURCEFUL_HH

#include <string>
#include <iostream>

namespace rsf {
  //! Return absolute path to directory holding the library
  //  Would not normally expose this, so purely for demo
  std::string getLibraryDir();

  //! Return absolute path to root directory for resources
  std::string getResourceRootDir();

  //! Return absolute path to root directory for plugins
  std::string getPluginRootDir();

  //! Cat the given resource to the supplied ostream
  void catResource(const std::string& resource, std::ostream& os);

  //! Run all available plugins on supplied sink
  void runPlugins(std::ostream& sink);
} // namespace rsf

#endif // RESOURCEFUL_HH

