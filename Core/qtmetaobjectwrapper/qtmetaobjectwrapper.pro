CONFIG  += console c++11 nuria
QT      += testlib
QT      -= gui
NURIA   += core

TARGET   = tst_runtimemetaobject
CONFIG  -= app_bundle

TEMPLATE = app

SOURCES += tst_qtmetaobjectwrapper.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
