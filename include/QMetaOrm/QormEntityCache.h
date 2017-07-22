#pragma once

#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QVariant>

class QMETAORM_LIBRARY_API QormEntityCache {
public:
    typedef QSharedPointer<QormEntityCache> Ptr;

public:
    virtual ~QormEntityCache() { }

    virtual bool contains(const QVariant& key, const QormMetaEntity::Ptr& mapping) = 0;

    virtual QVariant get(const QVariant& key, const QormMetaEntity::Ptr& mapping) = 0;

    virtual void put(const QVariant& key, const QVariant& item, const QormMetaEntity::Ptr& mapping) = 0;
};
