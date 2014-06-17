CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET    = tst_sessiontest
CONFIG   -= app_bundle

TEMPLATE  = app

SOURCES += tst_sessiontest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
