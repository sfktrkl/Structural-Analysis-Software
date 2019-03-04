QT       += core gui opengl
QT       += xml
LIBS += -lopengl32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SAS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

# Windows icon and admin right(not yet)
win32 {
    RC_FILE = sas.rc
}

DISTFILES += \
    sas.manifest


INCLUDEPATH += $$PWD/../SASStaticLibrary
DEPENDPATH += $$PWD/../SASStaticLibrary

INCLUDEPATH += $$PWD/../SASLibrary
DEPENDPATH += $$PWD/../SASLibrary

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SASLibrary/release/ -lSASLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SASLibrary/debug/ -lSASLibrary
else:unix: LIBS += -L$$OUT_PWD/../SASLibrary/ -lSASLibrary

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SASStaticLibrary/release/ -lSASStaticLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SASStaticLibrary/debug/ -lSASStaticLibrary
else:unix: LIBS += -L$$OUT_PWD/../SASStaticLibrary/ -lSASStaticLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASLibrary/release/libSASLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASLibrary/debug/libSASLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASLibrary/release/SASLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASLibrary/debug/SASLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../SASLibrary/libSASLibrary.a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/release/libSASStaticLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/debug/libSASStaticLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/release/SASStaticLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/debug/SASStaticLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../SASStaticLibrary/libSASStaticLibrary.a

HEADERS += \
    sas.rc
