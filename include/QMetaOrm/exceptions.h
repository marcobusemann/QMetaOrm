#pragma once

#include <QMetaOrm/private.h>
#include <QtSql>

/**
 * @brief The DatabaseException class
 */
class QMETAORM_LIBRARY_API DatabaseException: public std::runtime_error
{
public:
   DatabaseException(const QSqlError &error): runtime_error(error.text().toLocal8Bit()){}
};

/**
 * @brief The ConnectToDatabaseException class
 */
class QMETAORM_LIBRARY_API ConnectToDatabaseException: public DatabaseException
{
public:
   ConnectToDatabaseException(const QSqlError &error): DatabaseException(error){}
};

/**
 * @brief The TransactionException class
 */
class QMETAORM_LIBRARY_API TransactionException: public DatabaseException
{
public:
   TransactionException(const QSqlError &error): DatabaseException(error){}
};

/**
 * @brief The QueryException class
 */
class QMETAORM_LIBRARY_API QueryException: public DatabaseException
{
public:
   QueryException(const QSqlError &error): DatabaseException(error){}
};

/**
 * @brief The CouldNotPrepareQueryException class
 */
class QMETAORM_LIBRARY_API CouldNotPrepareQueryException: public QueryException
{
public:
   CouldNotPrepareQueryException(const QSqlError &error): QueryException(error){}
};

/**
 * @brief The CouldNotExecuteQueryException class
 */
class QMETAORM_LIBRARY_API CouldNotExecuteQueryException: public QueryException
{
public:
   CouldNotExecuteQueryException(const QSqlError &error): QueryException(error){}
};
