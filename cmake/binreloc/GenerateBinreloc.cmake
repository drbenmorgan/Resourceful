#.rst:
#
get_filename_component(_GENERATE_BINRELOC_MODULE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

function(generate_binreloc _name)
  # _name must be a valid C identifier
  if(NOT (_name MATCHES "[_a-zA-Z][_a-zA-Z0-9]*"))
    message(FATAL_ERROR "generate_binreloc: prefix for generated target/code must be a valid C-identifier")
  endif()
  set(BR_NAME "${_name}_binreloc")

  # Add interface target, not object as
  # - might need to know PIC or not...
  # .. but can prevent object being installed...
  # Do it first as this will ensure an error if the target already exists
  # without possibly overwriting files
  add_library(${BR_NAME} INTERFACE)

  # Need to uniquely mangle symbols
  string(RANDOM LENGTH 16 MANGLE_BINRELOC)
  set(MANGLE_BINRELOC "${BR_NAME}${MANGLE_BINRELOC}")

  # Configure .h and .c files
  set(BR_HEADER_TEMPLATE "${_GENERATE_BINRELOC_MODULE_DIR}/_binreloc.h.in")
  set(BR_SOURCE_TEMPLATE "${_GENERATE_BINRELOC_MODULE_DIR}/_binreloc.c.in")
  set(BR_GEN_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${BR_NAME}.h")
  set(BR_GEN_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${BR_NAME}.c")
  configure_file("${BR_HEADER_TEMPLATE}" "${BR_GEN_HEADER}" @ONLY)
  configure_file("${BR_SOURCE_TEMPLATE}" "${BR_GEN_SOURCE}" @ONLY)

  # Add sources and usage requirements to target/sources
  target_sources(${BR_NAME} INTERFACE "${BR_GEN_HEADER}" "${BR_GEN_SOURCE}")
  target_include_directories(${BR_NAME} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")
  set_source_files_properties("${BR_GEN_SOURCE}"
    PROPERTIES COMPILE_DEFINITIONS ENABLE_BINRELOC
    )
endfunction()

