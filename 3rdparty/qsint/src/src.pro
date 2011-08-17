include (src.pri)


HEADERS += \
    recentfiles.h \
    colorgrid.h \
    colorbutton.h \
    actionlabel.h \
    actionbox.h \
    colordefs.h \
    scrollpanner.h \
    widgetpopup.h \
    tablistmenu.h \
    actionpanel/actionpanel.h \
    actionpanel/actiongroup.h \
    actionpanel/taskheader_p.h \
    actionpanel/taskgroup_p.h \
    actionpanel/actionpanelscheme.h \
    actionpanel/winxppanelscheme.h \
    actionpanel/winvistapanelscheme.h

SOURCES += \
    recentfiles.cpp \
    colorgrid.cpp \
    colorbutton.cpp \
    actionlabel.cpp \
    actionbox.cpp \
    scrollpanner.cpp \
    widgetpopup.cpp \
    tablistmenu.cpp \
    actionpanel/actionpanel.cpp \
    actionpanel/actionpanelscheme.cpp \
    actionpanel/taskheader_p.cpp \
    actionpanel/taskgroup_p.cpp \
    actionpanel/actiongroup.cpp \
    actionpanel/winxppanelscheme.cpp \
    actionpanel/winvistapanelscheme.cpp

INCLUDEPATH += ./ ./actionpanel

OTHER_FILES += \
    ../include/QSint

RESOURCES += \
    actionpanel/schemes.qrc
