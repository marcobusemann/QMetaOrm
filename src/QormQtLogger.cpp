#include "QormQtLogger.h"
#include <QDebug>

QormLogger::Ptr QormQtLogger::factory()
{
    return QormLogger::Ptr(new QormQtLogger());
}

void QormQtLogger::trace(const QString& message)
{
    qDebug() << message;
}

void QormQtLogger::debug(const QString& message)
{
    qDebug() << message;
}

void QormQtLogger::info(const QString& message)
{
    qDebug() << message;
}

void QormQtLogger::warn(const QString& message)
{
    qWarning() << message;
}

void QormQtLogger::error(const QString& message)
{
    qFatal("%s", message.toLocal8Bit().data());
}

void QormQtLogger::fatal(const QString& message)
{
    qFatal("%s", message.toLocal8Bit().data());
}
