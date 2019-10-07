# Configure binreloc and relative path setup
generate_binreloc(RSFLibrary)
configure_file(Config_build.h.in Config_build.h @ONLY)

# Important that we use a SHARED library
add_library(Resourceful SHARED Resourceful.h Resourceful.cpp Filesystem.h)

target_include_directories(Resourceful
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Resourceful
  PUBLIC Boost::filesystem
  PRIVATE RSFLibrary_binreloc)

# Install the library and headers
install(TARGETS Resourceful
  # EXPORT helps us to create sets of exported/imported targets
  EXPORT ResourcefulTargets
  # Separate destinations help installation of multiple
  # targets (programs/libs) in one go, plus portability (DLLs
  # on Windows)
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(FILES Resourceful.h Filesystem.h
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Resourceful")

# Add plugins/tests
add_subdirectory(plugins)
