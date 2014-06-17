CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET    = tst_sessionmanagertest
CONFIG   -= app_bundle

TEMPLATE  = app

SOURCES += tst_sessionmanagertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
