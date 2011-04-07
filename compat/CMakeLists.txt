project( compat )

set(compat_SOURCES
    actioncollection.cpp
    standardaction.cpp
    iconloader.cpp
    utils.cpp
 )

if( HAVE_KDE )
    include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} )
    add_definitions( -DHAVE_KDE ${KDE4_DEFINITIONS} )

    kde4_add_library( compat ${compat_SOURCES} )
    target_link_libraries( compat ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS} )
else( HAVE_KDE )
    include( ${QT_USE_FILE} )
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/qtingkde
        ${CMAKE_CURRENT_SOURCE_DIR}/qtingkde/KDE
    )

    set( compat_SOURCES
        qtingkde/kcolorbutton.cpp
        qtingkde/kcolorcombo.cpp
        ${compat_SOURCES}
    )

    automoc4_add_library(compat ${compat_SOURCES})
    target_link_libraries(compat ${QT_LIBRARIES} )
endif( HAVE_KDE )


if( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  set_target_properties(compat PROPERTIES COMPILE_FLAGS "-fPIC")
endif( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

