CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET = tst_callback
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_callback.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
