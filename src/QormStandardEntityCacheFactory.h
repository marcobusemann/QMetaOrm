#pragma once

#include <QMetaOrm/QormEntityCacheFactory.h>

class QormStandardEntityCacheFactory : public QormEntityCacheFactory {
public:
    static QormEntityCacheFactory::Ptr factory();

public:
    virtual QormEntityCache::Ptr createCache() const override;
};