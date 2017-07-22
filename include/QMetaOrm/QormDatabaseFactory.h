#pragma once

#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QSqlDatabase>

class QMETAORM_LIBRARY_API QormDatabaseFactory {
public:
    typedef QSharedPointer<QormDatabaseFactory> Ptr;

public:
    virtual ~QormDatabaseFactory() { }

    virtual QSqlDatabase createDatabase(const QString& name = QString()) const = 0;
};
