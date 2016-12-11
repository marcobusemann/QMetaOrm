#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T14:31:06
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = MetaEntityTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

SOURCES += MetaEntityTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../QMetaOrm.pri)
