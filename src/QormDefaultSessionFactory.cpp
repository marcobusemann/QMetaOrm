#include "QormDefaultSessionFactory.h"
#include "QormDefaultSession.h"

QormDefaultSessionFactory::QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory, const QormLogger::Ptr &logger)
    :databaseFactory(databaseFactory)
     , logger(logger) { }

QormSession::Ptr QormDefaultSessionFactory::createSession() const
{
    return QSharedPointer<QormSession>(new QormDefaultSession(databaseFactory, logger));
}

QormDefaultSessionFactory::Ptr QormDefaultSessionFactory::factory(const QormDatabaseFactory::Ptr& databaseFactory, const QormLogger::Ptr &logger)
{
    return Ptr(new QormDefaultSessionFactory(databaseFactory, logger));
}
