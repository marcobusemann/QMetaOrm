#pragma once

#include <QMetaOrm\private.h>

#include <QSharedPointer>
#include <QString>

namespace QMetaOrm {
   /**
     * Provides a logging contract to use custom logging mechanism for the library.
     * Just write an adapter for your concrete logger and pass it to the requirering instances.
     */
   class QMETAORM_LIBRARY_API Logger
   {
   public:
      typedef QSharedPointer<Logger> Ptr;

   public:
      virtual ~Logger() {}
      virtual void trace(const QString &message) = 0;
      virtual void debug(const QString &message) = 0;
      virtual void info(const QString &message) = 0;
      virtual void warn(const QString &message) = 0;
      virtual void error(const QString &message) = 0;
      virtual void fatal(const QString &message) = 0;
   };

   /**
     * Simple logger which uses:
     * - qDebug for trace
     * - qDebug for debug
     * - qDebug for info
     * - qWarn for warn
     * - qFatal for error
     * - qFatal for fatal
     */
   class QMETAORM_LIBRARY_API StandardQtLogger : public Logger
   {
   public:
      static Logger::Ptr factory();

   public:
      virtual void trace(const QString &message) override;
      virtual void debug(const QString &message) override;
      virtual void info(const QString &message) override;
      virtual void warn(const QString &message) override;
      virtual void error(const QString &message) override;
      virtual void fatal(const QString &message) override;
   };
}