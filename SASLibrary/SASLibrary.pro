#-------------------------------------------------
#
# Project created by QtCreator 2019-03-02T17:25:18
#
#-------------------------------------------------

QT       += widgets qml quick opengl sql script scripttools xml xmlpatterns testlib
QT       += core gui opengl
QT       += xml
LIBS     += -lopengl32
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = SASLibrary
TEMPLATE = lib
DEFINES += SASLIBRARY_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        saslibrary.cpp \
        mainwindow.cpp \
    node.cpp \
    member.cpp \
    memberdata.cpp \
    structuredata.cpp \
    forcedata.cpp \
    glwidget.cpp \
    mainwindowstatics.cpp

HEADERS += \
        saslibrary.h \
        saslibrary_global.h \
        mainwindow.h \
    node.h \
    member.h \
    memberdata.h \
    structuredata.h \
    forcedata.h \
    glwidget.h

FORMS += \
        mainwindow.ui \
    memberdata.ui \
    structuredata.ui \
    forcedata.ui
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SASStaticLibrary/release/ -lSASStaticLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SASStaticLibrary/debug/ -lSASStaticLibrary
else:unix: LIBS += -L$$OUT_PWD/../SASStaticLibrary/ -lSASStaticLibrary

INCLUDEPATH += $$PWD/../SASStaticLibrary
DEPENDPATH += $$PWD/../SASStaticLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/release/libSASStaticLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/debug/libSASStaticLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/release/SASStaticLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/debug/SASStaticLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/libSASStaticLibrary.a
