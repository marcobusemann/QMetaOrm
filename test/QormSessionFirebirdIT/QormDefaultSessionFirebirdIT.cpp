#include "QormDefaultSessionIT.h"
#include "DatabaseFactory.h"

class QormDefaultSessionFirebirdIT : public QormDefaultSessionIT
{
    Q_OBJECT
private:
    virtual QormDatabaseFactory::Ptr databaseFactory() const override
    {
        return FirebirdDatabaseFactory::factory();
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionFirebirdIT)

#include "QormDefaultSessionFirebirdIT.moc"
