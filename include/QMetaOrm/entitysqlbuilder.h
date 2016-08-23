#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/metaentity.h>
#include <QMetaOrm/criterion.h>

#include <QSharedPointer>
#include <QStringList>
#include <QtSql>

#include <algorithm>

namespace QMetaOrm {

   /**
     *
     */
   class QMETAORM_LIBRARY_API EntitySqlBuilder
   {
   public:
      typedef QSharedPointer<EntitySqlBuilder> Ptr;

   public:
      virtual ~EntitySqlBuilder() {}

      virtual QString buildSelect(MetaEntity mapping);
      virtual QString buildCriterion(MetaEntity mapping, Criterion::Ptr criterion, QVariantList &conditions);
      virtual QString buildSelectMany(MetaEntity mapping, Criterion::Ptr criterion, int skip, int pageSize, QVariantList &conditions);
      virtual QString buildRemove(MetaEntity mapping);
      virtual QString buildInsert(MetaEntity mapping, QStringList &properties);
      virtual QString buildUpdate(MetaEntity mapping, QStringList &properties);
   };
}
