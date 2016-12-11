#include <QMetaOrm/SessionFactory.h>
#include <QMetaOrm/EntityMapper.h>
#include <QMetaOrm/EntitySqlBuilder.h>

using namespace QMetaOrm;

DefaultSessionFactory::DefaultSessionFactory(const DatabaseFactory::Ptr &databaseFactory, const Logger::Ptr &logger, const EntityCacheFactory::Ptr &cacheFactory)
   : m_databaseFactory(databaseFactory)
   , m_converterStore(DefaultConverterStore::factory())
   , m_logger(logger)
   , m_cacheFactory(cacheFactory)
{}

Session::Ptr DefaultSessionFactory::createSession() const {
   return QSharedPointer<Session>(new Session(
      m_databaseFactory,
      QSharedPointer<EntitySqlBuilder>(new EntitySqlBuilder()),
      QSharedPointer<EntityMapper>(new EntityMapper(m_logger, m_cacheFactory->createCache())),
      m_converterStore));
}

ConverterStore::Ptr DefaultSessionFactory::getConverterStore() const
{
   return m_converterStore;
}

SessionFactory::Ptr DefaultSessionFactory::factory(const DatabaseFactory::Ptr &databaseFactory, const Logger::Ptr &logger, const EntityCacheFactory::Ptr &cacheFactory) {
   return SessionFactory::Ptr(new DefaultSessionFactory(
      databaseFactory,
      logger != nullptr ? logger : StandardQtLogger::factory(),
      cacheFactory != nullptr ? cacheFactory : StandardEntityCacheFactory::factory()));
}
