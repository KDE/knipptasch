TEMPLATE = app

include(../qsint.pri)

INCLUDEPATH = ../../include

LIBS += -L../../lib -l$$LIBNAME

message("examples.pri:")
message($$CONFIG)
message($$LIBNAME)

CONFIG(release,debug|release){
    PRE_TARGETDEPS += ../../lib/libqsint.a
}

CONFIG(debug,debug|release){
    PRE_TARGETDEPS += ../../lib/libqsintd.a
}

