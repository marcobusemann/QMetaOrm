#include "QormDefaultSessionIT.h"
#include "DatabaseFactory.h"

class QormDefaultSessionSqliteIT : public QormDefaultSessionIT
{
    Q_OBJECT
private:

    virtual QormDatabaseFactory::Ptr databaseFactory() const override
    {
        return SQLiteEmbeddedDatabaseFactory::factory();
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionSqliteIT)

#include "QormDefaultSessionSqliteIT.moc"
