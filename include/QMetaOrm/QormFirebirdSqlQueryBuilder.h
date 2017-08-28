#pragma once

#include <QMetaOrm/QormSqlQueryBuilder.h>

#include <QSharedPointer>
#include <QStringList>

class QMETAORM_LIBRARY_API QormFirebirdSqlQueryBuilder : public QormSqlQueryBuilder
{
public:
    virtual ~QormFirebirdSqlQueryBuilder() {}

    QString buildSequenceSelect(QormMetaEntity::Ptr mapping) override;
};
