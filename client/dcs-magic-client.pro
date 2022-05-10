QT       += widgets core gui quickwidgets positioning network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../lib/unitobject.cpp \
    ../lib/unitsmanager.cpp \
    backendmanager.cpp \
    connectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    recordsmanager.cpp \
    unitsmodel.cpp

HEADERS += \
    ../lib/unitobject.h \
    ../lib/unitsmanager.h \
    backendmanager.h \
    connectiondialog.h \
    mainwindow.h \
    recordsmanager.h \
    unitsmodel.h \
    utils.h

FORMS += \
    connectiondialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc

DISTFILES += \
    BaseTrack.qml \
    main.qml
