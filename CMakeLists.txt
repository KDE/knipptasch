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


find_package( Qt4 4.6.0 COMPONENTS QtCore QtGui QtXml QtSvg QtTest REQUIRED )

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


if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS_DEBUGFULL "${CMAKE_CXX_FLAGS_DEBUGFULL} -g3 -fno-inline" )
    set( CMAKE_C_FLAGS_DEBUGFULL   "${CMAKE_CXX_FLAGS_DEBUGFULL} -g3 -fno-inline" )
endif( CMAKE_COMPILER_IS_GNUCXX )

option( CMAKE_COMPILE_GCOV "Build with coverage support." FALSE )

if( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER MATCHES "icc" )
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=c99 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wnon-virtual-dtor -ansi -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-check-new -fno-common" )

    if( CMAKE_COMPILE_GCOV )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage" )
    endif( CMAKE_COMPILE_GCOV )
endif( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER MATCHES "icc" )

if( CMAKE_COMPILER_IS_MINGW )
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -U__STRICT_ANSI__" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U__STRICT_ANSI__" )
endif( CMAKE_COMPILER_IS_MINGW )


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
        ${CMAKE_CURRENT_SOURCE_DIR}/compat/qtingkde
        ${CMAKE_CURRENT_SOURCE_DIR}/compat/qtingkde/KDE
    )

    set( KNIPPTASCH_DATA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/apps/knipptasch )
endif( HAVE_KDE )

include( config-destination.cmake )

message( STATUS "Knipptasch ${VERSION} will be built for install into ${KNIPPTASCH_INSTALL_PREFIX}" )

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_BINARY_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_BINARY_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
    ${CMAKE_CURRENT_BINARY_DIR}/3rdparty
    ${QCA2_INCLUDE_DIR}
    ${QT_INCLUDES}
 )


macro_optional_add_subdirectory( cmake )
add_subdirectory( 3rdparty )
add_subdirectory( data )
add_subdirectory( compat )
add_subdirectory( core )
add_subdirectory( gui )
#add_subdirectory( plugins )

macro_display_feature_log()


if( WIN32 )
    install( FILES ${QT_BINARY_DIR}/mingwm10.dll DESTINATION ${KNIPPTASCH_LIB_INSTALL_DIR} )

    install( FILES ${QT_LIBRARY_DIR}/QtCore4.dll DESTINATION ${KNIPPTASCH_LIB_INSTALL_DIR} )
    install( FILES ${QT_LIBRARY_DIR}/QtGui4.dll DESTINATION ${KNIPPTASCH_LIB_INSTALL_DIR} )
    #install( FILES ${QT_LIBRARY_DIR}/QtSvg4.dll DESTINATION ${KNIPPTASCH_LIB_INSTALL_DIR} )
    #install( FILES ${QT_LIBRARY_DIR}/QtXml4.dll DESTINATION ${KNIPPTASCH_LIB_INSTALL_DIR} )

    file( GLOB qtImageFormats_FILES "${QT_PLUGINS_DIR}/imageformats/*.dll" )
    install( FILES ${qtImageFormats_FILES} DESTINATION ${KNIPPTASCH_INSTALL_PREFIX}/imageformats )

    file( GLOB qtqm_FILES ${QT_TRANSLATIONS_DIR}/qt_*.qm )
    install( FILES ${qtqm_FILES} DESTINATION ${KNIPPTASCH_LOCALE_INSTALL_DIR} )

    file( GLOB qtconfigqm_FILES ${QT_TRANSLATIONS_DIR}/qtconfig_*.qm )
    install( FILES ${qtconfigqm_FILES} DESTINATION ${KNIPPTASCH_LOCALE_INSTALL_DIR} )

    configure_file( knipptasch.nsi.cmake ${KNIPPTASCH_INSTALL_PREFIX} @ONLY )

    # TODO
    #if( QCA2_FOUND )
    #    install( FILES ${QT_LIBRARY_DIR}/qca2.dll DESTINATION ${KNIPPTASCH_LIB_INSTALL_DIR} )
    #endif( QCA2_FOUND )
endif( WIN32 )
