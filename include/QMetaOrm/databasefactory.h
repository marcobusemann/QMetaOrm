#pragma once

#include <QMetaOrm/Private.h>

#include <QSharedPointer>
#include <QSqlDatabase>

namespace QMetaOrm {

   /**
    * @brief The DatabaseFactory class
    */
   class QMETAORM_LIBRARY_API DatabaseFactory
   {
   public:
       typedef QSharedPointer<DatabaseFactory> Ptr;

   public:
       virtual ~DatabaseFactory() {}
       virtual QSqlDatabase createDatabase(const QString &name = QString()) const = 0;
   };
}