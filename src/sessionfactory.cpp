#include <QMetaOrm/sessionfactory.h>
#include <QMetaOrm/entitymapper.h>
#include <QMetaOrm/entitysqlbuilder.h>

using namespace QMetaOrm;

DefaultSessionFactory::DefaultSessionFactory(DatabaseFactory::Ptr databaseFactory)
    : m_databaseFactory(databaseFactory)
    , m_converterStore(DefaultConverterStore::factory())
{}

Session::Ptr DefaultSessionFactory::createSession() const {
   return QSharedPointer<Session>(new Session(
      m_databaseFactory,
      QSharedPointer<EntitySqlBuilder>(new EntitySqlBuilder()),
      QSharedPointer<EntityMapper>(new EntityMapper()),
	  m_converterStore));
}

ConverterStore::Ptr DefaultSessionFactory::getConverterStore() const
{
	return m_converterStore;
}

SessionFactory::Ptr DefaultSessionFactory::factory(DatabaseFactory::Ptr databaseFactory) {
    return SessionFactory::Ptr(new DefaultSessionFactory(databaseFactory));
}
