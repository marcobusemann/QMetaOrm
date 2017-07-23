#pragma once

#include <QMetaOrm/QormLogger.h>

/**
  * Simple logger which uses:
  * - qDebug for trace
  * - qDebug for debug
  * - qDebug for info
  * - qWarn for warn
  * - qFatal for error
  * - qFatal for fatal
  */
class QormQtLogger : public QormLogger {
public:
    static QormLogger::Ptr factory();

public:
    virtual void trace(const QString& message) override;

    virtual void debug(const QString& message) override;

    virtual void info(const QString& message) override;

    virtual void warn(const QString& message) override;

    virtual void error(const QString& message) override;

    virtual void fatal(const QString& message) override;
};
