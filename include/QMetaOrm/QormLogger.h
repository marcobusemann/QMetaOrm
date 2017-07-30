#pragma once

#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QString>

/**
  * Provides a logging contract to use custom logging mechanism for the library.
  * Just write an adapter for your concrete logger and pass it to the requirering instances.
  */
class QMETAORM_LIBRARY_API QormLogger {
public:
    typedef QSharedPointer<QormLogger> Ptr;

public:
    virtual ~QormLogger() { }

    virtual void trace(const QString& message) = 0;

    virtual void debug(const QString& message) = 0;

    virtual void info(const QString& message) = 0;

    virtual void warn(const QString& message) = 0;

    virtual void error(const QString& message) = 0;

    virtual void fatal(const QString& message) = 0;
};
