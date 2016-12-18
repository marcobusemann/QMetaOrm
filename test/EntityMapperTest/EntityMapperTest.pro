QT       += testlib sql
QT       -= gui

TARGET = EntityMapperTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

SOURCES += EntityMapperTest.cpp

include(../../QMetaOrm.pri)
