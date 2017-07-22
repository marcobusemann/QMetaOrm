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

class QMETAORM_LIBRARY_API QormStandardEntityCache : public QormEntityCache {
public:
    static QormEntityCache::Ptr factory();

public:
    virtual bool contains(const QVariant& key, const QormMetaEntity::Ptr& mapping) override;

    virtual QVariant get(const QVariant& key, const QormMetaEntity::Ptr& mapping) override;

    virtual void put(const QVariant& key, const QVariant& item, const QormMetaEntity::Ptr& mapping) override;

private:
    QHash<QVariant, QVariant>& forMapping(const QormMetaEntity::Ptr& mapping);

    QHash<QormMetaEntity::Ptr, QHash<QVariant, QVariant>> m_data;
};

QMETAORM_LIBRARY_API uint qHash(const QVariant& key);
