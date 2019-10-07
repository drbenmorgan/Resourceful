# 1. A plain application, using binreloc directly
generate_binreloc(RSFSimple)
add_executable(rsf_simple rsf_simple.cc)
target_link_libraries(rsf_simple PRIVATE RSFSimple_binreloc)

# 2. An application using a dynamic library that uses binreloc
add_executable(rsf_library rsf_library.cc)
target_link_libraries(rsf_library PRIVATE Resourceful)


