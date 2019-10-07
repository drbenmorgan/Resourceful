#include "Resourceful/Resourceful.h"
#include "Resourceful/Filesystem.h"

#include "Resourceful/RSFLibrary_binreloc.h"
#include "Resourceful/Config_build.h"

// Private implementation details
namespace {
  //! Convert BrInitError code to a string describing the error
  std::string BRErrorAsString(const BrInitError& err) {
    std::string errMsg;
    switch (err) {
    case BR_INIT_ERROR_NOMEM:
      errMsg = "Unable to open /proc/self/maps";
      break;
    case BR_INIT_ERROR_OPEN_MAPS:
      errMsg =  "Unable to read from /proc/self/maps";
      break;
    case BR_INIT_ERROR_READ_MAPS:
      errMsg = "The file format of /proc/self/maps";
      break;
    case BR_INIT_ERROR_INVALID_MAPS:
      errMsg = "The file format of /proc/self/maps is invalid";
      break;
    case BR_INIT_ERROR_DISABLED:
      errMsg = "Binary relocation disabled";
      break;
    default:
      errMsg = "Unknown BrInitError";
    }
    return errMsg;
  }

  void initBinReloc() {
    BrInitError err;
    int initOK = br_init_lib(&err);
    if(initOK != 1) {
      throw std::runtime_error("binreloc init failed: '" + BRErrorAsString(err) + "'");
    }
  }
}

namespace rsf {
std::string getLibraryDir() {
  initBinReloc();
  char* libDir = br_find_exe_dir("");
  rsf::filesystem::path rawLibDir{libDir};
  free(libDir);
  auto canonicalLibDir = rsf::filesystem::canonical(rawLibDir);
  return canonicalLibDir.string();
}

std::string getResourceRootDir() {
  rsf::filesystem::path basePath{getLibraryDir()};
  basePath /= rsf::build::getLibDirToResourceDir();
  auto absPath = rsf::filesystem::canonical(basePath);
  return absPath.string();
}

std::string getPluginRootDir() {
  rsf::filesystem::path basePath{getLibraryDir()};
  basePath /= rsf::build::getLibDirToPluginDir();
  auto absPath = rsf::filesystem::canonical(basePath);
  return absPath.string();
}

} // namespace rsf
