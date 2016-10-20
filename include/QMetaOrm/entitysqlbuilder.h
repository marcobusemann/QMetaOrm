#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/metaentity.h>
#include <QMetaOrm/criterion.h>

#include <QSharedPointer>
#include <QStringList>

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

      virtual QString buildSelect(MetaEntity::Ptr mapping);
      virtual QString buildCriterion(MetaEntity::Ptr mapping, Criterion::Ptr criterion, QVariantList &conditions);
      virtual QString buildSelectMany(MetaEntity::Ptr mapping, Criterion::Ptr criterion, int skip, int pageSize, QVariantList &conditions);
      virtual QString buildRemove(MetaEntity::Ptr mapping);
      virtual QString buildInsert(MetaEntity::Ptr mapping, QStringList &properties);
      virtual QString buildUpdate(MetaEntity::Ptr mapping, QStringList &properties);
   };
}
