#pragma once

#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormCache.h>

class QMETAORM_LIBRARY_API QormEntityCacheFactory {
public:
    typedef QSharedPointer<QormEntityCacheFactory> Ptr;

public:
    virtual ~QormEntityCacheFactory() { }

    virtual QormEntityCache::Ptr createCache() const = 0;
};

class QMETAORM_LIBRARY_API QormStandardEntityCacheFactory : public QormEntityCacheFactory {
public:
    static QormEntityCacheFactory::Ptr factory();

public:
    virtual QormEntityCache::Ptr createCache() const override;
};
