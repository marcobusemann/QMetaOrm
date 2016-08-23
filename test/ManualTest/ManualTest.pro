QT += core sql
QT -= gui

CONFIG += c++11

TARGET = ManualTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
DESTDIR = ../../bin



include(../../QMetaOrm.pri)

HEADERS += \
    main.h
