QT += core gui widgets qml
QT += statemachine
CONFIG += c++2b
RC_ICONS = icon/we_tp.ico

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lwecore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lwecore
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH +=../deps/WECore/include
include( ../deps/WECore/WECore.pri)
INCLUDEPATH +=\
    ../deps
SOURCES += \
    lightwidget.cpp \
    main.cpp \
    querymaindialog.cpp
HEADERS += \
    lightwidget.h \
    querymaindialog.h
FORMS += \
    querymaindialog.ui
RESOURCES += \
    resourse.qrc
