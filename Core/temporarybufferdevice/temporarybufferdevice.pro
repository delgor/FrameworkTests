CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET    = tst_temporarybufferdevice
CONFIG   -= app_bundle

TEMPLATE  = app

SOURCES  += tst_temporarybufferdevice.cpp
DEFINES  += SRCDIR=\\\"$$PWD/\\\"
