#pragma once

#include <QMetaOrm/Private.h>
#include <QMetaOrm/MetaEntity.h>
#include <QMetaOrm/Criterion.h>

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
      virtual QString buildSelectMany(MetaEntity::Ptr mapping, Criterion::Ptr criterion, int skip, int pageSize, QVariantList &conditions);
      virtual QString buildRemove(MetaEntity::Ptr mapping);
      virtual QString buildInsertForSequence(MetaEntity::Ptr mapping, QStringList &properties);
      virtual QString buildInsertForIdentity(MetaEntity::Ptr mapping, QStringList &properties);
      virtual QString buildUpdate(MetaEntity::Ptr mapping, QStringList &properties);
      virtual QString buildSequenceSelect(MetaEntity::Ptr mapping);
   };
}
