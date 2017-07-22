#include "QormStandardQtLogger.h"
#include <QDebug>

QormLogger::Ptr QormStandardQtLogger::factory()
{
    return QormLogger::Ptr(new QormStandardQtLogger());
}

void QormStandardQtLogger::trace(const QString& message)
{
    qDebug() << message;
}

void QormStandardQtLogger::debug(const QString& message)
{
    qDebug() << message;
}

void QormStandardQtLogger::info(const QString& message)
{
    qDebug() << message;
}

void QormStandardQtLogger::warn(const QString& message)
{
    qWarning() << message;
}

void QormStandardQtLogger::error(const QString& message)
{
    qFatal(message.toLocal8Bit().data());
}

void QormStandardQtLogger::fatal(const QString& message)
{
    qFatal(message.toLocal8Bit().data());
}
