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

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    include/QMetaOrm/private.h \
    include/QMetaOrm/metaentity.h \
    include/QMetaOrm/metaentitybuilder.h \
    include/QMetaOrm/criterion.h \
    include/QMetaOrm/valuecriterion.h \
    include/QMetaOrm/listcriterion.h \
    include/QMetaOrm/entitysqlbuilder.h \
    include/QMetaOrm/entitymapper.h \
    include/QMetaOrm/exceptions.h \
    include/QMetaOrm/session.h \
    include/QMetaOrm/databasefactory.h \
    include/QMetaOrm/sessionfactory.h \
    include/QMetaOrm/converter.h \
    include/QMetaOrm/converterstore.h \
    include/QMetaOrm/logger.h \
    include/QMetaOrm/cache.h \
    include/QMetaOrm/cachefactory.h \

SOURCES += \
    src/metaentitybuilder.cpp \
    src/criterion.cpp \
    src/valuecriterion.cpp \
    src/listcriterion.cpp \
    src/entitysqlbuilder.cpp \
    src/session.cpp \
    src/sessionfactory.cpp \
    src/metaentity.cpp \
    src/converter.cpp \
    src/converterstore.cpp \
    src/entitymapper.cpp \
	src/logger.cpp \
	src/cache.cpp \
	src/cachefactory.cpp \
