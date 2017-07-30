#include <QtTest>
#include <QtSql>

#include <QMetaOrm/QormDatabaseSettings.h>

const QString VALUE = "A";

class QormDatabaseSettingsTest : public QObject {
Q_OBJECT

private:
    QSqlDatabase connection;

private Q_SLOTS:

    void init()
    {
        connection = QSqlDatabase::addDatabase("QSQLITE", "dummy");
    };

    void cleanup()
    {
        connection = QSqlDatabase();
        QSqlDatabase::removeDatabase("dummy");
    }

    void applyTo_databaseNameSet_connectionHasDatabaseName()
    {
        auto settings = QormDatabaseSettings();
        settings.setDatabaseName(VALUE);

        settings.applyTo(&connection);

        QCOMPARE(VALUE, connection.databaseName());
    }

    void applyTo_hostNameSet_connectionHasHostName()
    {
        auto settings = QormDatabaseSettings();
        settings.setHostName(VALUE);

        settings.applyTo(&connection);

        QCOMPARE(VALUE, connection.hostName());
    }

    void applyTo_portSet_connectionHasPort()
    {
        auto settings = QormDatabaseSettings();
        settings.setPort(10);

        settings.applyTo(&connection);

        QCOMPARE(10, connection.port());
    }

    void applyTo_userNameSet_connectionHasUserName()
    {
        auto settings = QormDatabaseSettings();
        settings.setUserName(VALUE);

        settings.applyTo(&connection);

        QCOMPARE(VALUE, connection.userName());
    }

    void applyTo_passwordSet_connectionHasPassword()
    {
        auto settings = QormDatabaseSettings();
        settings.setPassword(VALUE);

        settings.applyTo(&connection);

        QCOMPARE(VALUE, connection.password());
    }
};

QTEST_APPLESS_MAIN(QormDatabaseSettingsTest)

#include "QormDatabaseSettingsTest.moc"
