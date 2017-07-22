#include <QormEntitySqlBuilder.h>
#include <QMetaOrm/QormSessionFactory.h>

#include "QormStandardQtLogger.h"

QormDefaultSessionFactory::QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory)
    :databaseFactory(databaseFactory)
     , logger(QormStandardQtLogger::factory()) { }

QormSession::Ptr QormDefaultSessionFactory::createSession() const
{
    return QSharedPointer<QormSession>(new QormSession(databaseFactory, logger));
}

void QormDefaultSessionFactory::setLogger(const QormLogger::Ptr& logger)
{
    this->logger = logger;
}

QormDefaultSessionFactory::Ptr QormDefaultSessionFactory::factory(const QormDatabaseFactory::Ptr& databaseFactory)
{
    return Ptr(new QormDefaultSessionFactory(databaseFactory));
}
