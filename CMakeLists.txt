project( knipptasch )

set( knipptasch_MAJOR_VERSION "0" )
set( knipptasch_MINOR_VERSION "9" )
set( knipptasch_PATCH_LEVEL   "50" )
set( knipptasch_VERSION_SUFFIX "" )

#############################################################################################

cmake_minimum_required(VERSION 2.6.2)

set( CMAKE_COLOR_MAKEFILE ON )
set( CMAKE_VERBOSE_MAKEFILE OFF )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON )
set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )
set( CMAKE_USE_RELATIVE_PATHS FALSE )
SET( CMAKE_SKIP_BUILD_RPATH  FALSE )
SET( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )

set( knipptasch_SHORT_VERSION  "${knipptasch_MAJOR_VERSION}.${knipptasch_MINOR_VERSION}" )
set( knipptasch_VERSION        "${knipptasch_SHORT_VERSION}.${knipptasch_PATCH_LEVEL}${knipptasch_VERSION_SUFFIX}" )

add_definitions("-DKNIPPTASCH_MAJOR_VERSION=\"${knipptasch_MAJOR_VERSION}\"")
add_definitions("-DKNIPPTASCH_MINOR_VERSION=\"${knipptasch_MINOR_VERSION}\"")
add_definitions("-DKNIPPTASCH_PATCH_LEVEL=\"${knipptasch_PATCH_LEVEL}\"")
add_definitions("-DKNIPPTASCH_SHORT_VERSION=\"${knipptasch_SHORT_VERSION}\"")
add_definitions("-DKNIPPTASCH_VERSION_SUFFIX=\"${knipptasch_VERSION_SUFFIX}\"")
add_definitions("-DKNIPPTASCH_VERSION=\"${knipptasch_VERSION}\"")

include(TranslateMacros)
include(MacroLogFeature)
include(MacroOptionalAddSubdirectory)
include(MacroEnsureOutOfSourceBuild)
include(MacroOptionalFindPackage)

if( NOT DISABLE_FORCE_OUT_OF_SOURCE_BUILD )
  macro_ensure_out_of_source_build( "Knipptasch requires an out of source build. Please create a separate build directory and run 'cmake path_to_sources [options]' there." )
endif( NOT DISABLE_FORCE_OUT_OF_SOURCE_BUILD )


find_package( Qt4 4.6.0 COMPONENTS QtCore QtGui QtXml QtSvg REQUIRED )

if( NOT WIN32 )
  set( SHARED_MIME_INFO_MINIMUM_VERSION "0.60" )
  find_package( SharedMimeInfo ${SHARED_MIME_INFO_MINIMUM_VERSION} )
  macro_log_feature( SHAREDMIMEINFO_FOUND "shared-mime-info" "The core MIME database used to represent types of files" "http://freedesktop.org/wiki/Software/shared-mime-info" TRUE ${SHARED_MIME_INFO_MINIMUM_VERSION} "" )
endif( NOT WIN32 )

macro_optional_find_package( QCA2 )
macro_log_feature(QCA2_FOUND "QCA2" "Qt Cryptographic Architecture" "http://delta.affinix.com/qca" FALSE "2.0.0" "Needed for the file encryption functionality.")

if( QCA2_FOUND )
  add_definitions( -DWITH_QCA2 ${QCA2_DEFINITIONS} )
else( QCA2_FOUND )
  set( QCA2_LIBRARIES "" )
  set( QCA2_INCLUDE_DIR "" )
endif( QCA2_FOUND )


if( WIN32 OR APPLE )
    set( QTONLY TRUE )
endif( WIN32 OR APPLE )

if( NOT DEFINED QTONLY )
    set( QTONLY FALSE )
endif( NOT DEFINED QTONLY )

if( QTONLY )
    message(STATUS "Knipptasch will be built WITHOUT KDE support.")
    set( HAVE_KDE FALSE CACHE BOOL "Build version with KDE support." )
else( QTONLY )
    set( KDE_MIN_VERSION 4.4 )
    find_package( KDE4 REQUIRED )
    message( STATUS "Knipptasch will be built WITH KDE support." )
    set( HAVE_KDE TRUE CACHE BOOL "Build version with KDE support." )
endif( QTONLY )


if( HAVE_KDE )
    find_package( MSGFMT REQUIRED )
    include( KDE4Defaults )
    include( MacroLibrary )

    include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} )

    add_definitions( -DHAVE_KDE ${KDE4_DEFINITIONS} )
    set( KNIPPTASCH_DATA_INSTALL_DIR ${DATA_INSTALL_DIR}/knipptasch )
else( HAVE_KDE )
    include( ${QT_USE_FILE} )
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/compat/qtingkde
        ${CMAKE_CURRENT_SOURCE_DIR}/src/compat/qtingkde/KDE
    )

    set( KNIPPTASCH_DATA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/apps/knipptasch )
endif( HAVE_KDE )


message( STATUS "Knipptasch ${VERSION} will be built for install into ${CMAKE_INSTALL_PREFIX}" )

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
    ${CMAKE_CURRENT_BINARY_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/3rdparty
    ${QCA2_INCLUDE_DIR}
    ${QT_INCLUDES}
 )


set(knipptasch_SOURCES
    src/preferences.cpp
    src/accountmodel.cpp
    src/accountsortfilterproxymodel.cpp
    src/accountwidget.cpp
    src/welcomewidget.cpp
    src/mainwindow.cpp
    src/savemodifieddialog.cpp
    src/accountsettingsdialog.cpp
    src/settingsdialog.cpp
    src/recentfilemenu.cpp
    src/quickreportpopup.cpp
    src/quickreportwidget.cpp
    src/passworddialog.cpp
    src/passwordwidget.cpp
    src/main.cpp

    src/export/csv/csvexportplugin.cpp
    src/export/csv/csvexportdialog.cpp

    src/import/csv/csvimportplugin.cpp
    src/import/csv/csvimportdialog.cpp
    src/import/demo/demoimportplugin.cpp

    src/delegate/datedelegate.cpp
    src/delegate/moneydelegate.cpp
    src/delegate/postingtextdelegate.cpp
    src/delegate/categorydelegate.cpp

    src/backend/account.cpp
    src/backend/posting.cpp
    src/backend/storage.cpp

    src/compat/actioncollection.cpp
    src/compat/standardaction.cpp
    src/compat/iconloader.cpp
    src/compat/datepickerpopup.cpp
    src/compat/dateedit.cpp
    src/compat/utils.cpp

    3rdparty/modeltest/modeltest.cpp
 )

set( knipptasch_HEADERS
    src/mainwindow.h
    src/accountwidget.h
    src/accountmodel.h
    src/accountsortfilterproxymodel.h
    src/welcomewidget.h
    src/aboutdialog.h
    src/savemodifieddialog.h
    src/accountsettingsdialog.h
    src/settingsdialog.h
    src/recentfilemenu.h
    src/quickreportpopup.h
    src/quickreportwidget.h
    src/passworddialog.h
    src/passwordwidget.h

    #src/export/csv/csvexportplugin.h
    src/export/csv/csvexportdialog.h

    #src/import/csv/csvimportplugin.h
    src/import/csv/csvimportdialog.h
    #src/import/demo/demoimportplugin.h

    src/delegate/datedelegate.h
    src/delegate/moneydelegate.h
    src/delegate/postingtextdelegate.h
    src/delegate/categorydelegate.h

    src/compat/actioncollection.h
    src/compat/datepickerpopup.h
    src/compat/dateedit.h

    3rdparty/modeltest/modeltest.h
 )

set( knipptasch_FORMS
    src/accountwidget.ui
    src/mainwindow.ui
    src/welcomewidget.ui
    src/savemodifieddialog.ui
    src/accountsettingsdialog.ui
    src/settingsdialog.ui
    src/quickreportwidget.ui
    src/passworddialog.ui
    src/passwordwidget.ui

    src/export/csv/csvexportdialog.ui
    src/import/csv/csvimportdialog.ui
)

set( knipptasch_RESOURCES
    data/icons/appicon.qrc
    data/icons/oxygen.qrc
    data/resources/resources.qrc
 )


if( HAVE_KDE )
    kde4_add_ui_files( knipptasch_SOURCES ${knipptasch_FORMS} )
    kde4_add_app_icon( knipptasch_SOURCES data/icons/hi*-app-knipptasch.png )

    kde4_add_executable( knipptasch ${knipptasch_SOURCES} )
    target_link_libraries( knipptasch ${KDE4_KDECORE_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTSQL_LIBRARY} ${QCA2_LIBRARIES} )

    install( TARGETS knipptasch ${INSTALL_TARGETS_DEFAULT_ARGS} )
else( HAVE_KDE )
    set( knipptasch_SOURCES
        src/aboutdialog.cpp
        src/compat/qtingkde/kcolorbutton.cpp
        ${knipptasch_SOURCES}
    )

    set( knipptasch_HEADERS
        src/aboutdialog.h
        src/compat/qtingkde/kcolorbutton.h
        ${knipptasch_HEADERS}
    )

    set( knipptasch_FORMS
        src/aboutdialog.ui
        ${knipptasch_FORMS}
    )

    qt4_wrap_ui( knipptasch_SOURCES ${knipptasch_FORMS} )
    qt4_wrap_cpp( knipptasch_SOURCES ${knipptasch_HEADERS} )
    qt4_add_resources( knipptasch_SOURCES ${knipptasch_RESOURCES} )

    if( WIN32 AND HAVE_DEPLOY )
    set( WIN32_EXE WIN32 )
        set( KNIPPTASCH_QT_MAINLIB ${QT_QTMAIN_LIBRARY} )
    endif( WIN32 AND HAVE_DEPLOY )

    add_executable( knipptasch ${WIN32_EXE} ${knipptasch_SOURCES} )
    target_link_libraries(knipptasch ${QT_LIBRARIES} ${QCA2_LIBRARIES} ${KNIPPTASCH_QT_MAINLIB} )

    if( WIN32 )
        install(TARGETS knipptasch RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
    else( WIN32 )
        install(TARGETS knipptasch RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
    endif( WIN32 )

    if( WIN32 )
        install( FILES ${QT_BINARY_DIR}/mingwm10.dll DESTINATION ${CMAKE_INSTALL_PREFIX} )

        install( FILES ${QT_LIBRARY_DIR}/QtCore4.dll DESTINATION ${CMAKE_INSTALL_PREFIX} )
        install( FILES ${QT_LIBRARY_DIR}/QtGui4.dll DESTINATION ${CMAKE_INSTALL_PREFIX} )

        #install( FILES ${QT_LIBRARY_DIR}/QtCored4.dll DESTINATION ${CMAKE_INSTALL_PREFIX} )
        #install( FILES ${QT_LIBRARY_DIR}/QtGuid4.dll DESTINATION ${CMAKE_INSTALL_PREFIX} )

        install( FILES ${QT_PLUGINS_DIR}/imageformats/qgif4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/imageformats )
        install( FILES ${QT_PLUGINS_DIR}/imageformats/qico4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/imageformats )
        install( FILES ${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/imageformats )
        install( FILES ${QT_PLUGINS_DIR}/imageformats/qmng4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/imageformats )
        install( FILES ${QT_PLUGINS_DIR}/imageformats/qsvg4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/imageformats )
        install( FILES ${QT_PLUGINS_DIR}/imageformats/qtiff4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/imageformats )

        file( GLOB QTQM_FILES ${QT_TRANSLATIONS_DIR}/qt_*.qm )
        install( FILES ${QTQM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/translations )

        file( GLOB QTCONFIGQM_FILES ${QT_TRANSLATIONS_DIR}/qtconfig_*.qm )
        install( FILES ${QTCONFIGQM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/translations )

        configure_file( knipptasch.nsi.cmake ${CMAKE_INSTALL_PREFIX}/knipptasch.nsi @ONLY )

        # TODO
        #if( QCA2_FOUND )
        #    install( FILES ${QT_LIBRARY_DIR}/qca2.dll DESTINATION ${CMAKE_INSTALL_PREFIX} )
        #endif( QCA2_FOUND )

    endif( WIN32 )
endif( HAVE_KDE )


macro_optional_add_subdirectory( cmake )
macro_optional_add_subdirectory( strigi-analyzer )
add_subdirectory( 3rdparty )
add_subdirectory( data )


macro_display_feature_log()
