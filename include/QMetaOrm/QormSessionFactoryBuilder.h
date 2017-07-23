#pragma once

#include <QMetaOrm/QormDatabaseSettings.h>
#include <QMetaOrm/QormSessionFactory.h>
#include <QMetaOrm/QormPrivate.h>

class QMETAORM_LIBRARY_API QormSessionFactoryDatabaseConnectionBuilder {
public:
    QormSessionFactoryDatabaseConnectionBuilder(const QormDatabaseFactory::Ptr& databaseFactory);

    QormSessionFactoryDatabaseConnectionBuilder withLogger(const QormLogger::Ptr& logger);

    QormSessionFactory::Ptr build() const;

private:
    QormDatabaseFactory::Ptr databaseFactory;
    QormLogger::Ptr logger;
};

class QMETAORM_LIBRARY_API QormSessionFactoryBuilder {
public:
    static QormSessionFactoryBuilder AFactory();

    QormSessionFactoryDatabaseConnectionBuilder withDatabase(const QormDatabaseFactory::Ptr& databaseFactory);
    QormSessionFactoryDatabaseConnectionBuilder withDatabase(const QString &driverName, const QormDatabaseSettings& databaseSettings);
};