project( knipptasch_description_accounttabwidget_plugin )

set( knipptasch_plugin_SOURCES
    descriptiontabplugin.cpp
    descriptiontabwidget.cpp
 )

set( knipptasch_plugin_HEADERS
    descriptiontabplugin.h
    descriptiontabwidget.h
 )

qt4_wrap_cpp( knipptasch_plugin_SOURCES ${knipptasch_plugin_HEADERS} )
add_library( knipptasch_description_accounttabwidget_plugin SHARED ${knipptasch_plugin_SOURCES} )
target_link_libraries( knipptasch_description_accounttabwidget_plugin ${QT_LIBRARIES} knipptasch compat )
set_target_properties( knipptasch_description_accounttabwidget_plugin PROPERTIES PREFIX "" )

install( TARGETS knipptasch_description_accounttabwidget_plugin DESTINATION ${KNIPPTASCH_PLUGIN_INSTALL_DIR} )
