CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET = tst_debug
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_debug.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
