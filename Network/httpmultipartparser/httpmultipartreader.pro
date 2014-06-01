CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core network

TARGET = tst_httpmultipartreader
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_httpmultipartreader.cpp
