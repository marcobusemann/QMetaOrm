#include <QMetaOrm/QormCacheFactory.h>

QormEntityCacheFactory::Ptr QormStandardEntityCacheFactory::factory()
{
    return QormEntityCacheFactory::Ptr(new QormStandardEntityCacheFactory());
}

QormEntityCache::Ptr QormStandardEntityCacheFactory::createCache() const
{
    return QormStandardEntityCache::factory();
}
