#include "QormDefaultSessionIT"
#include "DatabaseFactory.h"

class QormDefaultSessionSqliteIT : public QormDefaultSessionSqliteIT {
    Q_OBJECT
private:
    QormSessionFactory::Ptr m_sessionFactory;
    SqlHelper::Ptr m_sqlHelper;

public:
    QormDefaultSessionSqliteIT() { }

private:

    virtual QormDatabaseFactory databaseFactory() const override
    {
        return SQLiteEmbeddedDatabaseFactory::factory();
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionSqliteIT)

#include "QormDefaultSessionSqliteIT.moc"
