#include "QormSettingBasedDatabaseFactory.h"

#include <QMetaOrm/QormFirebirdSqlQueryBuilder.h>

QSqlDatabase QormSettingBasedDatabaseFactory::createDatabase(const QString& name) const
{
    if (!QSqlDatabase::contains(name)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(driverName, name);
        databaseSettings.applyTo(&db);
    }
    return QSqlDatabase::database(name, false);
}

QormSqlQueryBuilder::Ptr QormSettingBasedDatabaseFactory::createSqlQueryBuilder() const
{
    if (driverName == "QIBASE")
        return QormSqlQueryBuilder::Ptr(new QormFirebirdSqlQueryBuilder());

    return QormSqlQueryBuilder::Ptr(new QormSqlQueryBuilder());
}

QormDatabaseFactory::Ptr
QormSettingBasedDatabaseFactory::factory(const QString& driverName, const QormDatabaseSettings& databaseSettings)
{
    return Ptr(new QormSettingBasedDatabaseFactory(driverName, databaseSettings));
}

QormSettingBasedDatabaseFactory::QormSettingBasedDatabaseFactory(const QString& driverName,
                                                                 const QormDatabaseSettings& databaseSettings)
    : driverName(driverName)
    , databaseSettings(databaseSettings)
{
}
