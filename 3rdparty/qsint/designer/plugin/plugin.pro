include(../designer.pri)

INCLUDEPATH += ../../include

CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(qsintplugin)
TEMPLATE    = lib

HEADERS     = scrollpannerplugin.h \
    colorgridplugin.h \
    qsintplugin.h \
    colorbuttonplugin.h \
    actionboxplugin.h \
    actionlabelplugin.h \
    actionpanelplugin.h \
    actiongroupplugin.h
SOURCES     = scrollpannerplugin.cpp \
    colorgridplugin.cpp \
    qsintplugin.cpp \
    colorbuttonplugin.cpp \
    actionboxplugin.cpp \
    actionlabelplugin.cpp \
    actionpanelplugin.cpp \
    actiongroupplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. -lqsint

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target
