#include <QMetaOrm/sessionfactory.h>
#include <QMetaOrm/entitymapper.h>
#include <QMetaOrm/entitysqlbuilder.h>

using namespace QMetaOrm;

DefaultSessionFactory::DefaultSessionFactory(const DatabaseFactory::Ptr &databaseFactory, const Logger::Ptr &logger)
   : m_databaseFactory(databaseFactory)
   , m_converterStore(DefaultConverterStore::factory())
   , m_logger(logger)
{}

Session::Ptr DefaultSessionFactory::createSession() const {
   return QSharedPointer<Session>(new Session(
      m_databaseFactory,
      QSharedPointer<EntitySqlBuilder>(new EntitySqlBuilder()),
      QSharedPointer<EntityMapper>(new EntityMapper(m_logger)),
      m_converterStore));
}

ConverterStore::Ptr DefaultSessionFactory::getConverterStore() const
{
   return m_converterStore;
}

SessionFactory::Ptr DefaultSessionFactory::factory(const DatabaseFactory::Ptr &databaseFactory, const Logger::Ptr &logger) {
      return SessionFactory::Ptr(new DefaultSessionFactory(
         databaseFactory, 
         logger != nullptr ? logger : StandardQtLogger::factory()));
}
