TEMPLATE = app

include(../qsint.pri)

LIBS += -L../../lib -l$$LIBNAME

CONFIG(release,debug|release){
    PRE_TARGETDEPS += ../../lib/libqsint.a
}

CONFIG(debug,debug|release){
    PRE_TARGETDEPS += ../../lib/libqsintd.a
}

