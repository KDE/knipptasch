project( knipptasch_csv_export_plugin )

set( knipptasch_plugin_SOURCES
    csvexportdialog.cpp
    csvexportplugin.cpp
 )

set( knipptasch_plugin_HEADERS
    csvexportdialog.h
    csvexportplugin.h
 )

set( knipptasch_plugin_FORMS
    csvexportdialog.ui
 )


qt4_wrap_ui( knipptasch_plugin_SOURCES ${knipptasch_plugin_FORMS} )
qt4_wrap_cpp( knipptasch_plugin_SOURCES ${knipptasch_plugin_HEADERS} )

add_library( knipptasch_csv_export_plugin SHARED ${knipptasch_plugin_SOURCES} )
target_link_libraries( knipptasch_csv_export_plugin ${QT_LIBRARIES} knipptasch compat )

install( TARGETS knipptasch_csv_export_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
