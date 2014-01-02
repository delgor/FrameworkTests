CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET    = tst_variant
CONFIG   -= app_bundle

TEMPLATE  = app

SOURCES  += tst_variant.cpp
DEFINES  += SRCDIR=\\\"$$PWD/\\\"
