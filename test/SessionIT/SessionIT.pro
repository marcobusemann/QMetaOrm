#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T14:31:06
#
#-------------------------------------------------

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
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../QMetaOrm.pri)
