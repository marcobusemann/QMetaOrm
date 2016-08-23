#include <QMetaOrm/sessionfactory.h>
#include <QMetaOrm/entitymapper.h>
#include <QMetaOrm/entitysqlbuilder.h>

//-----------------------------------------------------------------------------
DefaultSessionFactory::DefaultSessionFactory(DatabaseFactory::Ptr databaseFactory)
    : m_databaseFactory(databaseFactory)
{}

//-----------------------------------------------------------------------------
Session::Ptr DefaultSessionFactory::createSession() const {
    return QSharedPointer<Session>(new Session(
        m_databaseFactory,
        QSharedPointer<EntitySqlBuilder>(new EntitySqlBuilder()),
        QSharedPointer<EntityMapper>(new EntityMapper())));
}

//-----------------------------------------------------------------------------
SessionFactory::Ptr DefaultSessionFactory::create(DatabaseFactory::Ptr databaseFactory) {
    return SessionFactory::Ptr(new DefaultSessionFactory(databaseFactory));
}
