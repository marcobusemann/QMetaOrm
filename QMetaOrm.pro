QT       -= gui
QT       += sql

TARGET = QMetaOrm
TEMPLATE = lib

DEFINES += QMETAORM_LIBRARY

INCLUDEPATH   += "$$PWD"/include

DESTDIR       += "$$PWD"/lib
DLLDESTDIR    += "$$PWD"/bin

MOC_DIR       += "$$PWD"/build
OBJECTS_DIR   += "$$PWD"/build

CONFIG += c++11

# Copy lib files for tests
unix {
    QMAKE_POST_LINK += cp "$$PWD"/lib/* "$$PWD"/bin/
}

HEADERS += \
    include/QMetaOrm/Private.h \
    include/QMetaOrm/MetaEntity.h \
    include/QMetaOrm/MetaEntityBuilder.h \
    include/QMetaOrm/Criterion.h \
    include/QMetaOrm/ValueCriterion.h \
    include/QMetaOrm/ListCriterion.h \
    include/QMetaOrm/EntitySqlBuilder.h \
    include/QMetaOrm/EntityMapper.h \
    include/QMetaOrm/Exceptions.h \
    include/QMetaOrm/Session.h \
    include/QMetaOrm/DatabaseFactory.h \
    include/QMetaOrm/SessionFactory.h \
    include/QMetaOrm/Converter.h \
    include/QMetaOrm/ConverterStore.h \
    include/QMetaOrm/Logger.h \
    include/QMetaOrm/Cache.h \
    include/QMetaOrm/CacheFactory.h \

SOURCES += \
    src/MetaEntityBuilder.cpp \
    src/Criterion.cpp \
    src/ValueCriterion.cpp \
    src/ListCriterion.cpp \
    src/EntitySqlBuilder.cpp \
    src/Session.cpp \
    src/SessionFactory.cpp \
    src/MetaEntity.cpp \
    src/Converter.cpp \
    src/ConverterStore.cpp \
    src/EntityMapper.cpp \
    src/Logger.cpp \
    src/Cache.cpp \
    src/CacheFactory.cpp \
