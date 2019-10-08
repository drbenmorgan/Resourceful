# 1. A plain application, using binreloc directly
generate_binreloc(RSFSimple)
add_executable(rsf_simple rsf_simple.cc)
target_link_libraries(rsf_simple PRIVATE RSFSimple_binreloc)

# 2. An application using a dynamic library that uses binreloc
add_executable(rsf_dynamic rsf_library.cc)
target_link_libraries(rsf_dynamic PRIVATE Resourceful)

# 3. An application using a static library that uses binreloc
add_executable(rsf_static rsf_library.cc)
target_link_libraries(rsf_static PRIVATE ResourcefulStatic)

# Install RPATH for Linux/Darwin (CMake does not yet support genexs
# for this property)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set_target_properties(rsf_dynamic PROPERTIES INSTALL_RPATH "@executable_path/${PROJECT_BINDIR_TO_LIBDIR}")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set_target_properties(rsf_dynamic PROPERTIES INSTALL_RPATH "\$ORIGIN/${PROJECT_BINDIR_TO_LIBDIR}")
endif()

# Install
install(TARGETS rsf_simple rsf_dynamic rsf_static
  EXPORT ResourcefulTargets
  DESTINATION "${CMAKE_INSTALL_BINDIR}")
