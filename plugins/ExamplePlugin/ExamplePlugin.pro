QT += core gui widgets

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++20
TARGET = ExamplePlugin

LIBS += -luser32 -lshell32

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR  = ../../src/debug/plugins/Example
    } else {
        DESTDIR  = ../../src/release/plugins/Example
    }
}
win32:CONFIG(release, debug|release): LIBS += -L../../src/release -lwecore
else:win32:CONFIG(debug, debug|release): LIBS += -L../../src/debug/ -lwecore
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

include(../../deps/WECore/WECore.pri)
INCLUDEPATH +=\
    ../../deps/
SOURCES += \
    exampledialog.cpp \
    exampleplugin.cpp \
    examplewidget.cpp
HEADERS += \
    exampledialog.h \
    exampleplugin.h \
    examplewidget.h
FORMS += \
    exampledialog.ui



