#pragma once

#include <QMetaOrm/QormPrivate.h>

#include <QSqlError>
#include <QDebug>

class QMETAORM_LIBRARY_API QormDatabaseException : public std::runtime_error {
public:
    QormDatabaseException(const QSqlError& error)
        :runtime_error(error.text().toLocal8Bit().data()) { }
};

class QMETAORM_LIBRARY_API QormConnectToDatabaseException : public QormDatabaseException {
public:
    QormConnectToDatabaseException(const QSqlError& error)
        :QormDatabaseException(error)
    {
        qDebug() << error;
    }
};

class QMETAORM_LIBRARY_API QormTransactionException : public QormDatabaseException {
public:
    QormTransactionException(const QSqlError& error)
        :QormDatabaseException(error) { }
};

class QMETAORM_LIBRARY_API QormQueryException : public QormDatabaseException {
public:
    QormQueryException(const QSqlError& error)
        :QormDatabaseException(error)
    {
        qDebug() << error;
    }
};

class QMETAORM_LIBRARY_API QormCouldNotPrepareQueryException : public QormQueryException {
public:
    QormCouldNotPrepareQueryException(const QSqlError& error)
        :QormQueryException(error) { }
};

class QMETAORM_LIBRARY_API QormCouldNotExecuteQueryException : public QormQueryException {
public:
    QormCouldNotExecuteQueryException(const QSqlError& error)
        :QormQueryException(error) { }
};

class QMETAORM_LIBRARY_API QormCreatingObjectByMetaObjectException : public std::runtime_error {
public:
    QormCreatingObjectByMetaObjectException(const char* type)
        :std::runtime_error(
        QString("Creating an instance of %1 was not possible. We need a std ctor.").arg(type).toLocal8Bit().data()) { }
};

class QMETAORM_LIBRARY_API QormConverterNotFoundException : public std::runtime_error {
public:
    QormConverterNotFoundException(const char* type)
        :std::runtime_error(QString("Converter with name %1 was not found.").arg(type).toLocal8Bit().data()) { }

    QormConverterNotFoundException(const QString& type)
        :std::runtime_error(QString("Converter with name %1 was not found.").arg(type).toLocal8Bit().data()) { }
};

class QMETAORM_LIBRARY_API QormMoreThanOneResultException : public std::runtime_error {
public:
    QormMoreThanOneResultException()
        :std::runtime_error("A statement returned more than one result where only was was expected.") { }
};

class QMETAORM_LIBRARY_API QormCouldNotQueryNextSequenceValueException : public QormQueryException {
public:
    QormCouldNotQueryNextSequenceValueException(const QSqlError& error)
        :QormQueryException(error) { }
};
