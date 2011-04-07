project( knipptasch_core )

set( knipptasch_core_MAJOR_VERSION ${knipptasch_MAJOR_VERSION} )
set( knipptasch_core_MINOR_VERSION ${knipptasch_MINOR_VERSION} )
set( knipptasch_core_PATCH_LEVEL   ${knipptasch_PATCH_LEVEL} )
set( knipptasch_core_VERSION_SUFFIX ${knipptasch_VERSION_SUFFIX} )

#############################################################################################

find_package( Qt4 4.6.0 COMPONENTS QtCore QtGui REQUIRED )
find_package( Automoc4 REQUIRED )

include( ${QT_USE_FILE} )

add_definitions( "-DMAKE_KNIPPTASCH_CORE_LIBRARY" )

set( knipptasch_core_SOURCES
    abstractaccounttabwidget.cpp

    accountmodel.cpp
    categorymodel.cpp
    accountsortfilterproxymodel.cpp

    dateedit.cpp
    datevalidator.cpp
    datepickerpopup.cpp
    categorycombobox.cpp
    accounttablewidget.cpp

    preferences.cpp

    plugin/plugin.cpp
    plugin/exportplugin.cpp
    plugin/importplugin.cpp
    plugin/accounttabplugin.cpp
    plugin/pluginmanager.cpp

    backend/object.cpp
    backend/category.cpp
    backend/account.cpp
    backend/attachment.cpp
    backend/posting.cpp
    backend/baseposting.cpp
    backend/subposting.cpp
    backend/storage.cpp
    backend/storageexception.cpp

    delegate/datedelegate.cpp
    delegate/moneydelegate.cpp
    delegate/postingtextdelegate.cpp
    delegate/categorydelegate.cpp
 )


if( HAVE_KDE )
  kde4_add_library( knipptasch SHARED ${knipptasch_core_SOURCES} )
  target_link_libraries( knipptasch ${QT_LIBRARIES} modeltest compat )
else( HAVE_KDE )
  automoc4_add_library( knipptasch SHARED ${knipptasch_core_SOURCES} )
  target_link_libraries( knipptasch ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} modeltest compat )
endif( HAVE_KDE )

set_target_properties(
    knipptasch
  PROPERTIES
    VERSION "${knipptasch_core_MAJOR_VERSION}.${knipptasch_core_MINOR_VERSION}.${knipptasch_core_PATCH_LEVEL}"
    SOVERSION "${knipptasch_core_MAJOR_VERSION}.${knipptasch_core_MINOR_VERSION}"
)


install( TARGETS knipptasch DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS} )
