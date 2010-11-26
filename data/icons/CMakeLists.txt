if( HAVE_KDE )
    kde4_install_icons( ${ICON_INSTALL_DIR} )
else( HAVE_KDE )
    if( NOT WIN32 )
        set(ICON_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/)

        install( FILES hi64-app-knipptasch.png  DESTINATION ${ICON_INSTALL_DIR}/64x64/apps/   RENAME knipptasch.png )
        install( FILES hi48-app-knipptasch.png  DESTINATION ${ICON_INSTALL_DIR}/48x48/apps/   RENAME knipptasch.png )
        install( FILES hi32-app-knipptasch.png  DESTINATION ${ICON_INSTALL_DIR}/32x32/apps/   RENAME knipptasch.png )
        install( FILES hi22-app-knipptasch.png  DESTINATION ${ICON_INSTALL_DIR}/22x22/apps/   RENAME knipptasch.png )
    endif( NOT WIN32 )
endif( HAVE_KDE )

