#include "QormSettingBasedDatabaseFactory.h"

QSqlDatabase QormSettingBasedDatabaseFactory::createDatabase(const QString& name) const
{
    if (!QSqlDatabase::contains(name)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(driverName, name);
        databaseSettings.applyTo(&db);
    }
    return QSqlDatabase::database(name, false);
}

QormDatabaseFactory::Ptr
QormSettingBasedDatabaseFactory::factory(const QString& driverName, const QormDatabaseSettings& databaseSettings)
{
    return Ptr(new QormSettingBasedDatabaseFactory(driverName, databaseSettings));
}

QormSettingBasedDatabaseFactory::QormSettingBasedDatabaseFactory(const QString& driverName,
                                                                 const QormDatabaseSettings& databaseSettings)
    :driverName(driverName)
     , databaseSettings(databaseSettings)
{
}
