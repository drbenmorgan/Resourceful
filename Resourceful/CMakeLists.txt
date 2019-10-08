# Configure binreloc and relative path setup
generate_binreloc(RSFLibrary)
configure_file(Config_build.h.in Config_build.h @ONLY)

# Important that we use a SHARED library
add_library(Resourceful SHARED Resourceful.h Resourceful.cpp Config_build.h)

target_include_directories(Resourceful
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Resourceful
  PUBLIC Boost::filesystem
  PRIVATE RSFLibrary_binreloc ${CMAKE_DL_LIBS})

# We can build a ststic library, but this won't result in the right paths!
# We build the binreloc interfaces as an object library in this case, otherwise
# CMake requires us to export the RSFLibrary_binreloc target.
add_library(ResourcefulStatic_binreloc OBJECT)
target_link_libraries(ResourcefulStatic_binreloc PRIVATE RSFLibrary_binreloc)

add_library(ResourcefulStatic STATIC Resourceful.h Resourceful.cpp $<TARGET_OBJECTS:ResourcefulStatic_binreloc>)
set_target_properties(ResourcefulStatic PROPERTIES OUTPUT_NAME Resourceful)

# Here we use Boost via the old-style variables for include paths...
target_include_directories(ResourcefulStatic
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  ${Boost_INCLUDE_DIRS})

# ... and libraries
target_link_libraries(ResourcefulStatic
  PUBLIC ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
  PRIVATE ${CMAKE_DL_LIBS})

# Install the library and headers
install(TARGETS Resourceful ResourcefulStatic
  # EXPORT helps us to create sets of exported/imported targets
  EXPORT ResourcefulTargets
  # Separate destinations help installation of multiple
  # targets (programs/libs) in one go, plus portability (DLLs
  # on Windows)
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(FILES Resourceful.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Resourceful")

# Add plugins/tests
add_subdirectory(plugins)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()