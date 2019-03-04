QT       += core gui qml quick opengl sql script scripttools xml xmlpatterns testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on depend_includepath testcase
TEMPLATE = app
SOURCES +=  tst_testcase.cpp

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SASLibrary/release/ -lSASLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SASLibrary/debug/ -lSASLibrary
else:unix: LIBS += -L$$OUT_PWD/../SASLibrary/ -lSASLibrary

INCLUDEPATH += $$PWD/../SASLibrary
DEPENDPATH += $$PWD/../SASLibrary

DISTFILES += \
    test1.xml \
    test2.xml
