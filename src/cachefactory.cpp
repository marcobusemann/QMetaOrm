#include <QMetaOrm\CacheFactory.h>

using namespace QMetaOrm;

EntityCacheFactory::Ptr StandardEntityCacheFactory::factory()
{
   return EntityCacheFactory::Ptr(new StandardEntityCacheFactory());
}

EntityCache::Ptr StandardEntityCacheFactory::createCache() const
{
   return StandardEntityCache::factory();
}