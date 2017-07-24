#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QtSql>

class SqlHelper {
public:
    typedef QSharedPointer<SqlHelper> Ptr;

    static Ptr factory(QormDatabaseFactory::Ptr databaseFactory)
    {
        return Ptr(new SqlHelper(databaseFactory));
    }

public:
    SqlHelper(QormDatabaseFactory::Ptr databaseFactory)
        :
        m_databaseFactory(databaseFactory)
    {
    }

    void insert(const QString& aSql, const QVariantList& params = QVariantList())
    {
        auto database = m_databaseFactory->createDatabase();
        QSqlQuery query(database);
        bool okPrepare = query.prepare(aSql);
        Q_ASSERT(okPrepare);

        for (int i = 0; i<params.size(); i++)
            query.bindValue(i, params[i]);
        bool okExec = query.exec();
        Q_ASSERT(okExec);
    }

    QList<QVariantList> select(const QString& aSql, const QVariantList& params = QVariantList())
    {
        auto database = m_databaseFactory->createDatabase();
        QSqlQuery query(database);
        bool okPrepare = query.prepare(aSql);
        Q_ASSERT(okPrepare);

        for (int i = 0; i<params.size(); i++)
            query.bindValue(i, params[i]);
        bool okExec = query.exec();
        Q_ASSERT(okExec);

        QList<QVariantList> result;
        while (query.next()) {
            QVariantList record;
            for (int i = 0; i<query.record().count(); i++)
                record << query.value(i);
            result << record;
        }
        return result;
    }

private:
    QormDatabaseFactory::Ptr m_databaseFactory;
};