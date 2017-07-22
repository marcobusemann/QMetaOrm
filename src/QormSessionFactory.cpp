#include <QMetaOrm/QormEntitySqlBuilder.h>
#include <QMetaOrm/QormSessionFactory.h>
#include <QMetaOrm/QormEntityMapper.h>

#include "QormStandardEntityCacheFactory.h"

QormDefaultSessionFactory::QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory)
    :databaseFactory(databaseFactory)
     , converterStore(QormDefaultConverterStore::factory())
     , logger(QormStandardQtLogger::factory())
     , cacheFactory(QormStandardEntityCacheFactory::factory()) { }

QormSession::Ptr QormDefaultSessionFactory::createSession() const
{
    return QSharedPointer<QormSession>(new QormSession(
        databaseFactory,
        QSharedPointer<QormEntitySqlBuilder>(new QormEntitySqlBuilder()),
        QSharedPointer<QormEntityMapper>(new QormEntityMapper(logger, cacheFactory->createCache())),
        converterStore));
}

void QormDefaultSessionFactory::setLogger(const QormLogger::Ptr& logger)
{
    this->logger = logger;
}

void QormDefaultSessionFactory::setEntityCacheFactory(const QormEntityCacheFactory::Ptr& cacheFactory)
{
    this->cacheFactory = cacheFactory;
}

void QormDefaultSessionFactory::setConverterStore(const QormConverterStore::Ptr& converterStore)
{
    this->converterStore = converterStore;
}

QormDefaultSessionFactory::Ptr QormDefaultSessionFactory::factory(const QormDatabaseFactory::Ptr& databaseFactory)
{
    return Ptr(new QormDefaultSessionFactory(databaseFactory));
}
