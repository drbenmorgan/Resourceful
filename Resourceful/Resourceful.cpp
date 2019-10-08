#include "Resourceful/Resourceful.h"

#include "Resourceful/RSFLibrary_binreloc.h"
#include "Resourceful/Config_build.h"

// Use Boost for Filesystem
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <fstream>
#include <dlfcn.h>

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
  fs::path rawLibDir{libDir};
  free(libDir);
  auto canonicalLibDir = fs::canonical(rawLibDir);
  return canonicalLibDir.string();
}

std::string getResourceRootDir() {
  fs::path basePath{getLibraryDir()};
  basePath /= rsf::build::getLibDirToResourceDir();
  auto absPath = fs::canonical(basePath);
  return absPath.string();
}

std::string getPluginRootDir() {
  fs::path basePath{getLibraryDir()};
  basePath /= rsf::build::getLibDirToPluginDir();
  auto absPath = fs::canonical(basePath);
  return absPath.string();
}

void catResource(const std::string& resource, std::ostream& os) {
  auto resPath = fs::path{getResourceRootDir()} / resource;
  if (!fs::exists(resPath)) {
    std::cerr << "catResource : non-existant resource '" << resPath << "'" << std::endl;
    return;
  }
  if (!fs::is_regular_file(resPath)) {
    std::cerr << "catResource : resource '" << resPath << "; is not a regular file" << std::endl;
    return;
  }

  std::string sLine;
  std::ifstream resStream{resPath.string()};
  if (!resStream) {
    std::cerr << "catResource : failed to open stream on '" << resPath << "'" << std::endl;
    return;
  }
  os << "-- Resource '" << resPath << "'" << std::endl;
  while (std::getline(resStream,sLine)) {
    os << sLine << std::endl;
  }
  os << "--" << std::endl;
}

void runPlugins(std::ostream& sink) {
  using PluginFunction = void (*)(std::ostream&);
  const char* dlErrorMsg{nullptr};

  for(const std::string& plugin : {"cat", "dog", "parrot", "horse"}) {
    auto pluginPath = fs::path{getPluginRootDir()} / (plugin + ".so");
    
    sink << "Trying plugin '" << plugin << "' (" << pluginPath << ")" << std::endl;

    if(!fs::exists(pluginPath)) {
      std::cerr << "runPlugins: No such plugin '" << pluginPath <<"'" << std::endl;
      continue;
    }

    void* handle = dlopen(pluginPath.string().c_str(), RTLD_LAZY);
    dlErrorMsg = dlerror();

    if (handle == nullptr || dlErrorMsg != nullptr) {
      std::cerr << "runPlugins : dlerror: " << dlErrorMsg << std::endl;
      continue;
    }

    PluginFunction callable = (PluginFunction) dlsym(handle, "sayHello");
    dlErrorMsg = dlerror();

    if (callable == nullptr || dlErrorMsg != nullptr) {
      std::cerr << "runPlugins: dlerror: " << dlErrorMsg << std::endl;
      dlclose(handle);
      continue;
    }

    sink << plugin << " says ";
    callable(sink);
    sink << std::endl;

    int closed = dlclose(handle);
    dlErrorMsg = dlerror();

    if (closed < 0 || dlErrorMsg != nullptr) {
      std::cerr << "runPlugins: dlerror: " << dlErrorMsg << std::endl;
    }
  }
}

} // namespace rsf
