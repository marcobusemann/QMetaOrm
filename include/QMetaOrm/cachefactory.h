#pragma once

#include <QMetaOrm\private.h>
#include <QMetaOrm\cache.h>

namespace QMetaOrm {

   /**
   */
   class QMETAORM_LIBRARY_API EntityCacheFactory
   {
   public:
      typedef QSharedPointer<EntityCacheFactory> Ptr;

   public:
      virtual ~EntityCacheFactory() {}
      virtual EntityCache::Ptr createCache() const = 0;
   };

   /**
   */
   class QMETAORM_LIBRARY_API StandardEntityCacheFactory : public EntityCacheFactory
   {
   public:
      static EntityCacheFactory::Ptr factory();

   public:
      virtual EntityCache::Ptr createCache() const override;
   };
}