#pragma once

#include <QMetaOrm/QormMetaEntity.h>

#include <QSharedPointer>
#include <QVariant>

class QormEntityCache {
public:
    bool contains(const QVariant& key, const QormMetaEntity::Ptr& mapping);

    QVariant get(const QVariant& key, const QormMetaEntity::Ptr& mapping);

    void put(const QVariant& key, const QVariant& item, const QormMetaEntity::Ptr& mapping);

private:

    QHash<QVariant, QVariant>& forMapping(const QormMetaEntity::Ptr& mapping);

    QHash<QormMetaEntity::Ptr, QHash<QVariant, QVariant>> m_data;
};

uint qHash(const QVariant& key);
