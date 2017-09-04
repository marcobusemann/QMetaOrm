#pragma once

#include <QMetaOrm/QormDatabaseSettings.h>
#include <QMetaOrm/QormDatabaseFactory.h>

class QormSettingBasedDatabaseFactory : public QormDatabaseFactory {
public:
    static Ptr factory(const QString& driverName,
                       const QormDatabaseSettings& databaseSettings = QormDatabaseSettings());

    QSqlDatabase createDatabase(const QString& name) const override;
    QormSqlQueryBuilder::Ptr createSqlQueryBuilder() const override;

private:
    QormSettingBasedDatabaseFactory(const QString& driverName, const QormDatabaseSettings& databaseSettings);

    QString driverName;
    QormDatabaseSettings databaseSettings;
};
