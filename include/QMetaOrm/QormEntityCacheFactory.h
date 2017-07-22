#pragma once

#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormEntityCache.h>

class QMETAORM_LIBRARY_API QormEntityCacheFactory {
public:
    typedef QSharedPointer<QormEntityCacheFactory> Ptr;

public:
    virtual ~QormEntityCacheFactory() { }

    virtual QormEntityCache::Ptr createCache() const = 0;
};

