#include <QMetaOrm/sessionfactory.h>
#include <QMetaOrm/entitymapper.h>
#include <QMetaOrm/entitysqlbuilder.h>

using namespace QMetaOrm;

//-----------------------------------------------------------------------------
DefaultSessionFactory::DefaultSessionFactory(
   DatabaseFactory::Ptr databaseFactory,
   ConverterStoreFactory::Ptr converterStoreFactory)
    : m_databaseFactory(databaseFactory)
    , m_converterStoreFactory(converterStoreFactory)
{}

//-----------------------------------------------------------------------------
Session::Ptr DefaultSessionFactory::createSession() const {
   auto converterStoreFactory = m_converterStoreFactory != nullptr ? 
      m_converterStoreFactory : 
      DefaultConverterStoreFactory::factory();
   return QSharedPointer<Session>(new Session(
      m_databaseFactory,
      QSharedPointer<EntitySqlBuilder>(new EntitySqlBuilder()),
      QSharedPointer<EntityMapper>(new EntityMapper()),
      converterStoreFactory));
}

//-----------------------------------------------------------------------------
SessionFactory::Ptr DefaultSessionFactory::create(DatabaseFactory::Ptr databaseFactory, ConverterStoreFactory::Ptr converterStoreFactory) {
    return SessionFactory::Ptr(new DefaultSessionFactory(databaseFactory, converterStoreFactory));
}
