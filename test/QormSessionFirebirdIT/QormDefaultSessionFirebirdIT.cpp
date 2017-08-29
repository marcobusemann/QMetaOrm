#include "QormDefaultSessionIT"
#include "DatabaseFactory.h"

class QormDefaultSessionFirebirdIT : public QormDefaultSessionSqliteIT {
    Q_OBJECT
public:
    QormDefaultSessionFirebirdIT() { }

private:

    virtual QormDatabaseFactory databaseFactory() const override
    {
        return FirebirdDatabaseFactory::factory();
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionFirebirdIT)

#include "QormDefaultSessionFirebirdIT.moc"
