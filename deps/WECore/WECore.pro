QT -= gui

TARGET = wecore
TEMPLATE = lib

DEFINES += WIDGETEXPLORERSDK_LIBRARY

LIBS += -luser32 -lshell32

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR  = ../../src/debug
    } else {
        DESTDIR  = ../../src/release
    }
}

include(WECoreInt.pri)
HEADERS += \
    WDef/colordef.h \
    WDef/wedef.h \
    WE/we.h \
    wconfig.h \
    wdef.h \
    wfile.h \
    wplugin.h
SOURCES += \
    WE/we.cpp \
