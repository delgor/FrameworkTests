QT       += core testlib
QT       -= gui

TARGET    = tst_lazyconditionwalker
CONFIG   += console c++11 nuria
CONFIG   -= app_bundle
NURIA    += core

TEMPLATE  = app

SOURCES += tst_lazyconditionwalker.cpp
