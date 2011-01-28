project( knipptasch_csv_import_plugin )

set( knipptasch_plugin_SOURCES
    csvimportdialog.cpp
    csvimportplugin.cpp
 )

set( knipptasch_plugin_HEADERS
    csvimportdialog.h
    csvimportplugin.h
 )

set( knipptasch_plugin_FORMS
    csvimportdialog.ui
 )


qt4_wrap_ui( knipptasch_plugin_SOURCES ${knipptasch_plugin_FORMS} )
qt4_wrap_cpp( knipptasch_plugin_SOURCES ${knipptasch_plugin_HEADERS} )

add_library( knipptasch_csv_import_plugin SHARED ${knipptasch_plugin_SOURCES} )
target_link_libraries( knipptasch_csv_import_plugin ${QT_LIBRARIES} knipptasch compat )

install( TARGETS knipptasch_csv_import_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
