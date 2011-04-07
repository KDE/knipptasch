project( knipptasch_description_accounttabwidget_plugin )

set( knipptasch_plugin_SOURCES
    descriptiontabplugin.cpp
    descriptiontabwidget.cpp
 )

automoc4_add_library( knipptasch_description_accounttabwidget_plugin SHARED ${knipptasch_plugin_SOURCES} )
target_link_libraries( knipptasch_description_accounttabwidget_plugin ${QT_LIBRARIES} knipptasch compat )
set_target_properties( knipptasch_description_accounttabwidget_plugin PROPERTIES PREFIX "" )

install( TARGETS knipptasch_description_accounttabwidget_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
