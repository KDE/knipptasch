project( knipptasch_core )

set( knipptasch_core_MAJOR_VERSION ${knipptasch_MAJOR_VERSION} )
set( knipptasch_core_MINOR_VERSION ${knipptasch_MINOR_VERSION} )
set( knipptasch_core_PATCH_LEVEL   ${knipptasch_PATCH_LEVEL} )
set( knipptasch_core_VERSION_SUFFIX ${knipptasch_VERSION_SUFFIX} )

#############################################################################################

find_package( Qt4 4.6.0 COMPONENTS QtCore QtGui REQUIRED )
include( ${QT_USE_FILE} )

add_definitions( "-DMAKE_KNIPPTASCH_CORE_LIBRARY" )

set( knipptasch_core_SOURCES
    plugin.cpp
    exportplugin.cpp
    importplugin.cpp
    abstractaccounttabwidgetplugin.cpp

    abstractaccounttabwidget.cpp

    accountmodel.cpp
    categorymodel.cpp

    widgets/dateedit.cpp
    widgets/datevalidator.cpp
    widgets/datepickerpopup.cpp
    widgets/categorycombobox.cpp

    backend/object.cpp
    backend/category.cpp
    backend/account.cpp
    backend/attachment.cpp
    backend/posting.cpp
    backend/baseposting.cpp
    backend/subposting.cpp
    backend/storage.cpp
    backend/storageexception.cpp
 )


set( knipptasch_core_HEADERS
    plugin.h
    exportplugin.h
    importplugin.h
    abstractaccounttabwidgetplugin.h

    abstractaccounttabwidget.h

    accountmodel.h
    categorymodel.h

    widgets/dateedit.h
    widgets/datevalidator.h
    widgets/datepickerpopup.h
    widgets/categorycombobox.h

    backend/object.h
    backend/category.h
    backend/account.h
    backend/attachment.h
    backend/posting.h
    backend/baseposting.h
    backend/subposting.h
    backend/storage.h
    backend/storageexception.h
 )


if( HAVE_KDE )
  kde4_add_library( knipptasch SHARED ${knipptasch_core_SOURCES} )
  target_link_libraries( knipptasch ${QT_LIBRARIES} modeltest compat )
else( HAVE_KDE )
  qt4_wrap_cpp( knipptasch_core_SOURCES ${knipptasch_core_HEADERS} )
  add_library( knipptasch SHARED ${knipptasch_core_SOURCES} )
  target_link_libraries( knipptasch ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} modeltest compat )
endif( HAVE_KDE )

set_target_properties(
    knipptasch
  PROPERTIES
    VERSION "${knipptasch_core_MAJOR_VERSION}.${knipptasch_core_MINOR_VERSION}.${knipptasch_core_PATCH_LEVEL}"
    SOVERSION "${knipptasch_core_MAJOR_VERSION}.${knipptasch_core_MINOR_VERSION}"
)


install( TARGETS knipptasch DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS} )
