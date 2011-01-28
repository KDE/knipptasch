set( knipptasch_plugin_MAJOR_VERSION ${knipptasch_MAJOR_VERSION} )
set( knipptasch_plugin_MINOR_VERSION ${knipptasch_MINOR_VERSION} )
set( knipptasch_plugin_PATCH_LEVEL ${knipptasch_PATCH_LEVEL} )
set( knipptasch_plugin_VERSION_SUFFIX ${knipptasch_VERSION_SUFFIX} )

macro_optional_add_subdirectory( csv_export )
macro_optional_add_subdirectory( csv_import )
macro_optional_add_subdirectory( demo_import )
#macro_optional_add_subdirectory( xml )

