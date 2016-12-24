#pragma once

#include <QMetaOrm/Private.h>
#include <QSqlError>
#include <qdebug.h>

namespace QMetaOrm {

   /**
    */
   class QMETAORM_LIBRARY_API DatabaseException: public std::runtime_error
   {
   public:
      DatabaseException(const QSqlError &error): runtime_error(error.text().toLocal8Bit().data()){}
   };

   /**
    */
   class QMETAORM_LIBRARY_API ConnectToDatabaseException: public DatabaseException
   {
   public:
      ConnectToDatabaseException(const QSqlError &error): DatabaseException(error)
      {
         qDebug() << error;
      }
   };

   /**
    */
   class QMETAORM_LIBRARY_API TransactionException: public DatabaseException
   {
   public:
      TransactionException(const QSqlError &error): DatabaseException(error){}
   };

   /**
    */
   class QMETAORM_LIBRARY_API QueryException: public DatabaseException
   {
   public:
      QueryException(const QSqlError &error): DatabaseException(error)
      {
         qDebug() << error;
      }
   };

   /**
    */
   class QMETAORM_LIBRARY_API CouldNotPrepareQueryException: public QueryException
   {
   public:
      CouldNotPrepareQueryException(const QSqlError &error): QueryException(error) {}
   };

   /**
    */
   class QMETAORM_LIBRARY_API CouldNotExecuteQueryException: public QueryException
   {
   public:
      CouldNotExecuteQueryException(const QSqlError &error): QueryException(error) {}
   };

   /**
   */
   class QMETAORM_LIBRARY_API CreatingObjectByMetaObjectException : public std::runtime_error
   {
   public:
      CreatingObjectByMetaObjectException(const char *type) 
         : std::runtime_error(QString("Creating an instance of %1 was not possible. We need a std ctor.").arg(type).toLocal8Bit().data())
      {}
   };

   /**
   */
   class QMETAORM_LIBRARY_API ConverterNotFoundException : public std::runtime_error
   {
   public:
      ConverterNotFoundException(const char *type)
         : std::runtime_error(QString("Converter with name %1 was not found.").arg(type).toLocal8Bit().data())
      {}
      ConverterNotFoundException(const QString &type)
         : std::runtime_error(QString("Converter with name %1 was not found.").arg(type).toLocal8Bit().data())
      {}
   };

   /**
   */
   class QMETAORM_LIBRARY_API MoreThanOneResultException : public std::runtime_error
   {
   public:
      MoreThanOneResultException()
         : std::runtime_error("A statement returned more than one result where only was was expected.")
      {}
   };

   /**
    */
   class QMETAORM_LIBRARY_API CouldNotQueryNextSequenceValueException: public QueryException
   {
   public:
      CouldNotQueryNextSequenceValueException(const QSqlError &error): QueryException(error) {}
   };
}
