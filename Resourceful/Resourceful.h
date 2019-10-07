#ifndef RESOURCEFUL_HH
#define RESOURCEFUL_HH

#include <string>

namespace rsf {
  //! Return absolute path to directory holding the library
  //  Would not normally expose this, so purely for demo
  std::string getLibraryDir();

  //! Return absolute path to root directory for resources
  std::string getResourceRootDir();

  //! Return absolute path to root directory for plugins
  std::string getPluginRootDir();
} // namespace rsf

#endif // RESOURCEFUL_HH

