#pragma once

#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormMetaEntity.h>

#include <QSharedPointer>
#include <QStringList>

class QMETAORM_LIBRARY_API QormSqlQueryBuilder
{
public:
    typedef QSharedPointer<QormSqlQueryBuilder> Ptr;

public:
    virtual ~QormSqlQueryBuilder() { }

    virtual QString buildSelect(QormMetaEntity::Ptr mapping);

    virtual QString buildSelectMany(QormMetaEntity::Ptr mapping, int skip, int pageSize, QVariantList& conditions);

    virtual QString buildRemove(QormMetaEntity::Ptr mapping);

    virtual QString buildInsertForSequence(QormMetaEntity::Ptr mapping, QStringList& properties);

    virtual QString buildInsertForIdentity(QormMetaEntity::Ptr mapping, QStringList& properties);

    virtual QString buildUpdate(QormMetaEntity::Ptr mapping, QStringList& properties);

    virtual QString buildSequenceSelect(const QString &aSequence);
};
