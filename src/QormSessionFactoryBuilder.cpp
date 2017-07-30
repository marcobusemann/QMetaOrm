#include <QMetaOrm/QormSessionFactoryBuilder.h>

#include "QormSettingBasedDatabaseFactory.h"
#include "QormDefaultSessionFactory.h"
#include "QormQtLogger.h"

QormSessionFactoryBuilder QormSessionFactoryBuilder::AFactory()
{
    return QormSessionFactoryBuilder();
}

QormSessionFactoryDatabaseConnectionBuilder QormSessionFactoryBuilder::withDatabase(
    const QormDatabaseFactory::Ptr& databaseFactory)
{
    return QormSessionFactoryDatabaseConnectionBuilder(databaseFactory);
}

QormSessionFactoryDatabaseConnectionBuilder QormSessionFactoryBuilder::withDatabase(const QString& driverName,
                                                                                    const QormDatabaseSettings& databaseSettings)
{
    return withDatabase(QormSettingBasedDatabaseFactory::factory(driverName, databaseSettings));
}

QormSessionFactoryDatabaseConnectionBuilder::QormSessionFactoryDatabaseConnectionBuilder(
    const QormDatabaseFactory::Ptr& databaseFactory)
    :databaseFactory(databaseFactory)
     , logger(QormQtLogger::factory())
{
}

QormSessionFactoryDatabaseConnectionBuilder QormSessionFactoryDatabaseConnectionBuilder::withLogger(
    const QormLogger::Ptr& logger)
{
    this->logger = logger;
    return *this;
}

QormSessionFactory::Ptr QormSessionFactoryDatabaseConnectionBuilder::build() const
{
    return QormDefaultSessionFactory::factory(databaseFactory, logger);
}
