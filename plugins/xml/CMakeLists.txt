project( knipptasch_xml_plugin )


###################################################################################
#### knipptasch_xml_export_plugin
###################################################################################

set( knipptasch_export_plugin_SOURCES
    xmlexportplugin.cpp
    xmlwriter.cpp
 )

set( knipptasch_export_plugin_FORMS
    #xmlexportdialog.ui
 )

qt4_wrap_ui( knipptasch_export_plugin_SOURCES ${knipptasch_export_plugin_FORMS} )
automoc4_add_library( knipptasch_xml_export_plugin SHARED ${knipptasch_export_plugin_SOURCES} )
target_link_libraries( knipptasch_xml_export_plugin ${QT_LIBRARIES} knipptasch compat )
set_target_properties( knipptasch_xml_export_plugin PROPERTIES PREFIX "" )

install( TARGETS knipptasch_xml_export_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )



###################################################################################
#### knipptasch_xml_import_plugin
###################################################################################

set( knipptasch_import_plugin_SOURCES
    xmlimportplugin.cpp
    xmlreader.cpp
 )

set( knipptasch_import_plugin_FORMS
    #xmlimportdialog.ui
 )

qt4_wrap_ui( knipptasch_import_plugin_SOURCES ${knipptasch_import_plugin_FORMS} )
automoc4_add_library( knipptasch_xml_import_plugin SHARED ${knipptasch_import_plugin_SOURCES} )
target_link_libraries( knipptasch_xml_import_plugin ${QT_LIBRARIES} knipptasch compat )
set_target_properties( knipptasch_xml_import_plugin PROPERTIES PREFIX "" )

install( TARGETS knipptasch_xml_import_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
