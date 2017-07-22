#include <QMetaOrm/QormEntitySqlBuilder.h>
#include <QMetaOrm/QormSessionFactory.h>
#include <QMetaOrm/QormEntityMapper.h>

QormDefaultSessionFactory::QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory,
    const QormLogger::Ptr& logger, const QormEntityCacheFactory::Ptr& cacheFactory)
    :m_databaseFactory(databaseFactory), m_converterStore(QormDefaultConverterStore::factory()), m_logger(logger),
     m_cacheFactory(cacheFactory) { }

QormSession::Ptr QormDefaultSessionFactory::createSession() const
{
    return QSharedPointer<QormSession>(new QormSession(
        m_databaseFactory,
        QSharedPointer<QormEntitySqlBuilder>(new QormEntitySqlBuilder()),
        QSharedPointer<QormEntityMapper>(new QormEntityMapper(m_logger, m_cacheFactory->createCache())),
        m_converterStore));
}

QormConverterStore::Ptr QormDefaultSessionFactory::getConverterStore() const
{
    return m_converterStore;
}

QormSessionFactory::Ptr
QormDefaultSessionFactory::factory(const QormDatabaseFactory::Ptr& databaseFactory, const QormLogger::Ptr& logger,
    const QormEntityCacheFactory::Ptr& cacheFactory)
{
    return QormSessionFactory::Ptr(new QormDefaultSessionFactory(
        databaseFactory,
        logger!=nullptr ? logger : QormStandardQtLogger::factory(),
        cacheFactory!=nullptr ? cacheFactory : QormStandardEntityCacheFactory::factory()));
}
