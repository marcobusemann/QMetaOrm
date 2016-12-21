#include <QMetaOrm/Logger.h>
#include <QtGlobal>
#include <QDebug>

using namespace QMetaOrm;

Logger::Ptr StandardQtLogger::factory()
{
   return Logger::Ptr(new StandardQtLogger());
}

void StandardQtLogger::trace(const QString &message)
{
   qDebug() << message;
}

void StandardQtLogger::debug(const QString &message)
{
   qDebug() << message;
}

void StandardQtLogger::info(const QString &message)
{
   qDebug() << message;
}

void StandardQtLogger::warn(const QString &message)
{
   qWarning() << message;
}

void StandardQtLogger::error(const QString &message)
{
   qFatal(message.toLocal8Bit().data());
}

void StandardQtLogger::fatal(const QString &message)
{
   qFatal(message.toLocal8Bit().data());
}
