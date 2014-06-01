CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core network

TARGET    = tst_httpwriter
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_httpwriter.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
