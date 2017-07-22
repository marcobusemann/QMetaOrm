#pragma once

#include <QMetaOrm/QormEntityCache.h>

class QormStandardEntityCache : public QormEntityCache {
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

uint qHash(const QVariant& key);
