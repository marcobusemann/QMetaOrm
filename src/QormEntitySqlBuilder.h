#pragma once

#include <QMetaOrm/QormMetaEntity.h>

#include <QSharedPointer>
#include <QStringList>

#include <algorithm>

class QormEntitySqlBuilder {
public:
    typedef QSharedPointer<QormEntitySqlBuilder> Ptr;

public:
    virtual ~QormEntitySqlBuilder() { }

    virtual QString buildSelect(QormMetaEntity::Ptr mapping);

    virtual QString buildSelectMany(QormMetaEntity::Ptr mapping, int skip, int pageSize, QVariantList& conditions);

    virtual QString buildRemove(QormMetaEntity::Ptr mapping);

    virtual QString buildInsertForSequence(QormMetaEntity::Ptr mapping, QStringList& properties);

    virtual QString buildInsertForIdentity(QormMetaEntity::Ptr mapping, QStringList& properties);

    virtual QString buildUpdate(QormMetaEntity::Ptr mapping, QStringList& properties);

    virtual QString buildSequenceSelect(QormMetaEntity::Ptr mapping);
};
