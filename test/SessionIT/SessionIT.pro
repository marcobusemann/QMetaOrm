QT       += testlib sql
QT       -= gui

TARGET = SessionIT
CONFIG   += console
CONFIG   -= app_bundle

RESOURCES += resource.qrc

TEMPLATE = app
DESTDIR = ../../bin

HEADERS += Person.h DatabaseFactory.h
SOURCES += SessionIT.cpp Person.cpp

include(../../QMetaOrm.pri)
