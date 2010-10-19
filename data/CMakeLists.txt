add_subdirectory( icons )
add_subdirectory( resources )

if( HAVE_KDE ) 
    install( FILES   knipptasch.desktop        DESTINATION ${XDG_APPS_INSTALL_DIR} )
    install( FILES   knipptaschui.rc           DESTINATION ${KNIPPTASCH_DATA_INSTALL_DIR} )
else( HAVE_KDE )
    install(FILES   knipptasch.desktop         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications )
endif( HAVE_KDE )