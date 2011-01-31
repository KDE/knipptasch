project( knipptasch_csv_plugin )


###################################################################################
#### knipptasch_csv_export_plugin
###################################################################################

set( knipptasch_export_plugin_SOURCES
    csvexportdialog.cpp
    csvexportplugin.cpp
 )

set( knipptasch_export_plugin_HEADERS
    csvexportdialog.h
    csvexportplugin.h
 )

set( knipptasch_export_plugin_FORMS
    csvexportdialog.ui
 )

qt4_wrap_ui( knipptasch_export_plugin_SOURCES ${knipptasch_export_plugin_FORMS} )
qt4_wrap_cpp( knipptasch_export_plugin_SOURCES ${knipptasch_export_plugin_HEADERS} )

add_library( knipptasch_csv_export_plugin SHARED ${knipptasch_export_plugin_SOURCES} )
target_link_libraries( knipptasch_csv_export_plugin ${QT_LIBRARIES} knipptasch compat )

install( TARGETS knipptasch_csv_export_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )



###################################################################################
#### knipptasch_csv_import_plugin
###################################################################################

set( knipptasch_import_plugin_SOURCES
    csvimportdialog.cpp
    csvimportplugin.cpp
 )

set( knipptasch_import_plugin_HEADERS
    csvimportdialog.h
    csvimportplugin.h
 )

set( knipptasch_import_plugin_FORMS
    csvimportdialog.ui
 )

qt4_wrap_ui( knipptasch_import_plugin_SOURCES ${knipptasch_import_plugin_FORMS} )
qt4_wrap_cpp( knipptasch_import_plugin_SOURCES ${knipptasch_import_plugin_HEADERS} )

add_library( knipptasch_csv_import_plugin SHARED ${knipptasch_import_plugin_SOURCES} )
target_link_libraries( knipptasch_csv_import_plugin ${QT_LIBRARIES} knipptasch compat )

install( TARGETS knipptasch_csv_import_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
