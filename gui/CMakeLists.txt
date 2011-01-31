project( knipptasch )

set(knipptasch_SOURCES
    accountsortfilterproxymodel.cpp
    accountwidget.cpp
    welcomewidget.cpp
    mainwindow.cpp
    savemodifieddialog.cpp
    recentfilemenu.cpp
    quickreportpopup.cpp
    quickreportwidget.cpp
    passworddialog.cpp
    main.cpp

    config/configdialog.cpp
    config/configwidget.cpp
    config/abstractconfigpage.cpp
    config/accountconfigwidget.cpp
    config/accountconfigdialog.cpp
    config/accountcategoryconfigpage.cpp
    config/accountgeneralconfigpage.cpp
    config/accountpasswordconfigpage.cpp
    config/preferencesconfigdialog.cpp
    config/preferencesconfigwidget.cpp
    config/preferencesgeneralconfigpage.cpp
    config/preferencesappearanceconfigpage.cpp
    config/preferencesaccounttableconfigpage.cpp
    config/appearancecolorchooserwidget.cpp

    delegate/datedelegate.cpp
    delegate/moneydelegate.cpp
    delegate/postingtextdelegate.cpp
    delegate/categorydelegate.cpp
 )

set( knipptasch_HEADERS
    mainwindow.h
    accountwidget.h
    accountsortfilterproxymodel.h
    welcomewidget.h
    aboutdialog.h
    savemodifieddialog.h
    recentfilemenu.h
    quickreportpopup.h
    quickreportwidget.h
    passworddialog.h

    config/configdialog.h
    config/configwidget.h
    config/abstractconfigpage.h
    config/accountconfigwidget.h
    config/accountconfigdialog.h
    config/accountcategoryconfigpage.h
    config/accountgeneralconfigpage.h
    config/accountpasswordconfigpage.h
    config/preferencesconfigdialog.h
    config/preferencesconfigwidget.h
    config/preferencesgeneralconfigpage.h
    config/preferencesappearanceconfigpage.h
    config/preferencesaccounttableconfigpage.h
    config/appearancecolorchooserwidget.h

    delegate/datedelegate.h
    delegate/moneydelegate.h
    delegate/postingtextdelegate.h
    delegate/categorydelegate.h
 )

set( knipptasch_FORMS
    accountwidget.ui
    mainwindow.ui
    welcomewidget.ui
    savemodifieddialog.ui
    quickreportwidget.ui
    passworddialog.ui

    config/configwidget.ui
    config/accountcategoryconfigpage.ui
    config/accountgeneralconfigpage.ui
    config/accountpasswordconfigpage.ui
    config/preferencesgeneralconfigpage.ui
    config/preferencesappearanceconfigpage.ui
    config/preferencesaccounttableconfigpage.ui
    config/appearancecolorchooserwidget.ui
)

set( knipptasch_RESOURCES
    ../data/icons/appicon.qrc
    ../data/icons/oxygen.qrc
    ../data/resources/resources.qrc
 )


if( HAVE_KDE )
    #TODO build and install po files?

    kde4_add_ui_files( knipptasch_SOURCES ${knipptasch_FORMS} )
    kde4_add_app_icon( knipptasch_SOURCES ../data/icons/hi*-app-knipptasch.png )

    kde4_add_kcfg_files(knipptasch_SOURCES preferences.kcfgc)

    kde4_add_executable( knipptasch-bin ${knipptasch_SOURCES} )
    target_link_libraries( knipptasch-bin
        ${KDE4_KDECORE_LIBS}
        ${KDE4_KFILE_LIBS}
        ${KDE4_KIO_LIBS}
        ${KDE4_KDEUI_LIBS}
        ${QT_QTTEST_LIBRARY}
        ${QCA2_LIBRARIES}
        knipptasch compat modeltest
      )

    set_target_properties( knipptasch-bin PROPERTIES OUTPUT_NAME knipptasch )

    install( TARGETS knipptasch-bin ${INSTALL_TARGETS_DEFAULT_ARGS} )
else( HAVE_KDE )
    CONFIGURE_FILE( preferences_qt.h ${CMAKE_CURRENT_BINARY_DIR}/preferences.h COPYONLY)

    set( knipptasch_SOURCES
        aboutdialog.cpp
        ${knipptasch_SOURCES}
    )

    set( knipptasch_HEADERS
        aboutdialog.h
        ${knipptasch_HEADERS}
    )

    set( knipptasch_FORMS
        aboutdialog.ui
        ${knipptasch_FORMS}
    )

    file( GLOB TS_FILES "../l10n/knipptasch_*.ts" )
    qt4_add_translation( QM_FILES ${TS_FILES} )

    qt4_wrap_ui( knipptasch_SOURCES ${knipptasch_FORMS} )
    qt4_wrap_cpp( knipptasch_SOURCES ${knipptasch_HEADERS} )
    qt4_add_resources( knipptasch_SOURCES ${knipptasch_RESOURCES} )

    if( WIN32 AND HAVE_DEPLOY )
    set( WIN32_EXE WIN32 )
        set( KNIPPTASCH_QT_MAINLIB ${QT_QTMAIN_LIBRARY} )
    endif( WIN32 AND HAVE_DEPLOY )

    add_executable( knipptasch-bin ${WIN32_EXE} ${QM_FILES} ${knipptasch_SOURCES} )
    target_link_libraries( knipptasch-bin
          ${QT_LIBRARIES}
          ${QCA2_LIBRARIES}
          ${KNIPPTASCH_QT_MAINLIB}
          knipptasch
          compat
          modeltest
    )

    set_target_properties( knipptasch-bin PROPERTIES OUTPUT_NAME knipptasch )

    install( TARGETS knipptasch-bin ${INSTALL_TARGETS_DEFAULT_ARGS} )
    install( FILES ${QM_FILES} DESTINATION ${KNIPPTASCH_LOCALE_INSTALL_DIR} )
endif( HAVE_KDE )
