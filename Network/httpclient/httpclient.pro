CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core network

TARGET    = tst_httpparser
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH = ..

SOURCES += tst_httpclient.cpp \
    ../httpmemorytransport.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../httpmemorytransport.hpp
