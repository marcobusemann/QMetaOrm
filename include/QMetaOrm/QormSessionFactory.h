#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormCacheFactory.h>
#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormSession.h>
#include <QMetaOrm/QormLogger.h>

#include <QSharedPointer>

class QMETAORM_LIBRARY_API QormSessionFactory
{
public:
   typedef QSharedPointer<QormSessionFactory> Ptr;

public:
   virtual ~QormSessionFactory() {}
   virtual QormSession::Ptr createSession() const = 0;
   virtual QormConverterStore::Ptr getConverterStore() const = 0;
};

class QMETAORM_LIBRARY_API QormDefaultSessionFactory : public QormSessionFactory
{
public:
   static QormSessionFactory::Ptr factory(
      const QormDatabaseFactory::Ptr &databaseFactory,
      const QormLogger::Ptr &logger = QormLogger::Ptr(),
      const QormEntityCacheFactory::Ptr &cacheFactory = QormEntityCacheFactory::Ptr());

public:
   QormDefaultSessionFactory(
      const QormDatabaseFactory::Ptr &databaseFactory,
      const QormLogger::Ptr &logger,
      const QormEntityCacheFactory::Ptr &cacheFactory);

   virtual QormSession::Ptr createSession() const override;
   virtual QormConverterStore::Ptr getConverterStore() const override;

private:
   QormDatabaseFactory::Ptr m_databaseFactory;
   QormConverterStore::Ptr m_converterStore;
   QormLogger::Ptr m_logger;
   QormEntityCacheFactory::Ptr m_cacheFactory;
};
