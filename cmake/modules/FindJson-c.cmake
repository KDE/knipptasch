if( JSON-C_INCLUDE_DIR AND JSON-C_LIBRARIES )
  # in cache already
  set( JSON-C_FOUND TRUE )
else( JSON-C_INCLUDE_DIR AND JSON-C_LIBRARIES )
  find_library( JSON-C_LIBRARIES json )
  find_path( JSON-C_INCLUDE_DIR "json/json.h" )

  include( FindPackageHandleStandardArgs )
  find_package_handle_standard_args( JSON-C DEFAULT_MSG JSON-C_LIBRARIES JSON-C_INCLUDE_DIR )

  mark_as_advanced( JSON-C_INCLUDE_DIR JSON-C_LIBRARIES )

endif( JSON-C_INCLUDE_DIR AND JSON-C_LIBRARIES )
