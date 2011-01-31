project( knipptasch_xml_plugin )


###################################################################################
#### knipptasch_xml_export_plugin
###################################################################################

set( knipptasch_export_plugin_SOURCES
    xmlexportplugin.cpp
    xmlwriter.cpp
 )

set( knipptasch_export_plugin_HEADERS
    xmlexportplugin.h
    xmlwriter.h
 )

set( knipptasch_export_plugin_FORMS
    #xmlexportdialog.ui
 )

qt4_wrap_ui( knipptasch_export_plugin_SOURCES ${knipptasch_export_plugin_FORMS} )
qt4_wrap_cpp( knipptasch_export_plugin_SOURCES ${knipptasch_export_plugin_HEADERS} )

add_library( knipptasch_xml_export_plugin SHARED ${knipptasch_export_plugin_SOURCES} )
target_link_libraries( knipptasch_xml_export_plugin ${QT_LIBRARIES} knipptasch compat )

install( TARGETS knipptasch_xml_export_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )



###################################################################################
#### knipptasch_xml_import_plugin
###################################################################################

set( knipptasch_import_plugin_SOURCES
    xmlimportplugin.cpp
    xmlreader.cpp
 )

set( knipptasch_import_plugin_HEADERS
    xmlimportplugin.h
    xmlreader.h
 )

set( knipptasch_import_plugin_FORMS
    #xmlimportdialog.ui
 )

qt4_wrap_ui( knipptasch_import_plugin_SOURCES ${knipptasch_import_plugin_FORMS} )
qt4_wrap_cpp( knipptasch_import_plugin_SOURCES ${knipptasch_import_plugin_HEADERS} )

add_library( knipptasch_xml_import_plugin SHARED ${knipptasch_import_plugin_SOURCES} )
target_link_libraries( knipptasch_xml_import_plugin ${QT_LIBRARIES} knipptasch compat )

install( TARGETS knipptasch_xml_import_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
