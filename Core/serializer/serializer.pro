#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T20:52:12
#
#-------------------------------------------------

CONFIG   += console c++11 nuria
QT       += testlib
QT       -= gui
NURIA    += core

TARGET    = tst_serializer
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_serializer.cpp
HEADERS += structures.hpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
