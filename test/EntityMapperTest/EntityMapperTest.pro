#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T14:31:06
#
#-------------------------------------------------

QT       += testlib sql

QT       -= gui

TARGET = tst_entitymappertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

SOURCES += tst_entitymappertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../QMetaOrm.pri)
