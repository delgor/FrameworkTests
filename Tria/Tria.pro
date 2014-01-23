CONFIG  += console c++11 nuria
QT      += testlib
QT      -= gui
NURIA   += core

TARGET   = tst_tria
CONFIG  -= app_bundle

TEMPLATE = app

SOURCES += tst_tria.cpp
HEADERS += tst_header.hpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
