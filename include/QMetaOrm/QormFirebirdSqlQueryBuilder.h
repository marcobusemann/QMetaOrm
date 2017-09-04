#pragma once

#include <QMetaOrm/QormSqlQueryBuilder.h>

class QMETAORM_LIBRARY_API QormFirebirdSqlQueryBuilder : public QormSqlQueryBuilder
{
public:
    virtual ~QormFirebirdSqlQueryBuilder() {}

    QString buildSequenceSelect(QormMetaEntity::Ptr mapping) override;
};
