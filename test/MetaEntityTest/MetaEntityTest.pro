QT       += testlib
QT       -= gui

TARGET = MetaEntityTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

SOURCES += MetaEntityTest.cpp

include(../../QMetaOrm.pri)
