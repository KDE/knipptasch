project( knipptasch_demo_import_plugin )

set( knipptasch_plugin_SOURCES
    demoimportplugin.cpp
 )

set( knipptasch_plugin_HEADERS
    demoimportplugin.h
 )

qt4_wrap_cpp( knipptasch_plugin_SOURCES ${knipptasch_plugin_HEADERS} )
add_library( knipptasch_demo_import_plugin SHARED ${knipptasch_plugin_SOURCES} )
target_link_libraries( knipptasch_demo_import_plugin ${QT_LIBRARIES} knipptasch compat )
set_target_properties( knipptasch_demo_import_plugin PROPERTIES PREFIX "" )

install( TARGETS knipptasch_demo_import_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
