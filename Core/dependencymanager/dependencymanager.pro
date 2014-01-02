CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET    = tst_dependencymanager
CONFIG   -= app_bundle

TEMPLATE  = app

SOURCES  += tst_dependencymanager.cpp
DEFINES  += SRCDIR=\\\"$$PWD/\\\"
