project( qsint )

include_directories( src )

set(qsint_SOURCES
    src/colorgrid.cpp
    src/scrollpanner.cpp
    src/actionlabel.cpp
    src/actionbox.cpp
    src/actionpanel/actionpanel.cpp
    src/actionpanel/actiongroup.cpp
    src/actionpanel/winvistapanelscheme.cpp
    src/actionpanel/taskheader_p.cpp
    src/actionpanel/actionpanelscheme.cpp
    src/actionpanel/winxppanelscheme.cpp
    src/actionpanel/taskgroup_p.cpp
    src/colorbutton.cpp
    src/recentfiles.cpp
    src/tablistmenu.cpp
    src/widgetpopup.cpp
 )

QT4_ADD_RESOURCES( qsint_RES_SOURCES src/actionpanel/schemes.qrc )

automoc4_add_library(qsint SHARED ${qsint_SOURCES} ${qsint_RES_SOURCES} )
target_link_libraries(qsint ${QT_LIBRARIES} )
set_target_properties( qsint PROPERTIES VERSION "0.2.0" SOVERSION "0.2" )

if( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  set_target_properties(qsint PROPERTIES COMPILE_FLAGS "-fPIC")
endif( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )


install( TARGETS qsint DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS} )