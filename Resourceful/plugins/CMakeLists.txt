foreach(module cat dog parrot)
  add_library(${module} MODULE ${module}.cpp)
  # Hacky way of setting the output directory for plugins
  set_target_properties(${module} PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:Resourceful>/rsf"
    )
  install(TARGETS ${module} LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/rsf")
endforeach()
