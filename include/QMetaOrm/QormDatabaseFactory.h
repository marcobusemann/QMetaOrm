#pragma once

#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormSqlQueryBuilder.h>

#include <QSharedPointer>

class QSqlDatabase;

class QMETAORM_LIBRARY_API QormDatabaseFactory {
public:
    typedef QSharedPointer<QormDatabaseFactory> Ptr;

public:
    virtual ~QormDatabaseFactory() { }

    virtual QSqlDatabase createDatabase(const QString& name = QString()) const = 0;
    virtual QormSqlQueryBuilder::Ptr createSqlQueryBuilder() const = 0;
};
