#pragma once

#include <QMetaOrm/MetaEntity.h>
#include <QMetaOrm/Private.h>

#include <QSharedPointer>
#include <QVariant>

namespace QMetaOrm {

   class QMETAORM_LIBRARY_API EntityCache
   {
   public:
      typedef QSharedPointer<EntityCache> Ptr;

   public:
      virtual ~EntityCache() {}
      virtual bool contains(const QVariant &key, const MetaEntity::Ptr &mapping) = 0;
      virtual QVariant get(const QVariant &key, const MetaEntity::Ptr &mapping) = 0;
      virtual void put(const QVariant &key, const QVariant &item, const MetaEntity::Ptr &mapping) = 0;
   };

   class QMETAORM_LIBRARY_API StandardEntityCache : public EntityCache
   {
   public:
      static EntityCache::Ptr factory();

   public:
      virtual bool contains(const QVariant &key, const MetaEntity::Ptr &mapping) override;
      virtual QVariant get(const QVariant & key, const MetaEntity::Ptr &mapping) override;
      virtual void put(const QVariant & key, const QVariant & item, const MetaEntity::Ptr &mapping) override;

   private:
      QHash<QVariant, QVariant> &forMapping(const MetaEntity::Ptr &mapping);
      QHash<MetaEntity::Ptr, QHash<QVariant, QVariant>> m_data;
   };
}

QMETAORM_LIBRARY_API uint qHash(const QVariant &key);
