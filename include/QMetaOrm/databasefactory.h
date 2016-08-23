#pragma once

#include <QMetaOrm/private.h>

#include <QSharedPointer>
#include <QSqlDatabase>

/**
 * @brief The DatabaseFactory class
 */
class QMETAORM_LIBRARY_API DatabaseFactory
{
public:
    typedef QSharedPointer<DatabaseFactory> Ptr;

public:
    virtual ~DatabaseFactory() {}
    virtual QSqlDatabase createDatabase() const = 0;
};
