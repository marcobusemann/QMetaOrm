#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormSqlQueryBuilder.h>

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include <QFile>
#include <QDebug>

class SQLiteEmbeddedDatabaseFactory : public QormDatabaseFactory {
public:
    static Ptr factory()
    {
        return Ptr(new SQLiteEmbeddedDatabaseFactory());
    }

public:
    SQLiteEmbeddedDatabaseFactory()
        :m_isDatabaseInitialized(false)
    {
        m_file = QUuid::createUuid().toString().remove("{").remove("}").remove("-")+".sqlite";
    }

    ~SQLiteEmbeddedDatabaseFactory()
    {
        cleanup();
    }

    virtual QSqlDatabase createDatabase(const QString& name = QString()) const override
    {
        if (!QSqlDatabase::contains(name)) {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", name);
            db.setDatabaseName(m_file);

            if (!m_isDatabaseInitialized) {
                initializeDatabase(db);
                m_isDatabaseInitialized = true;
            }
        }
        return QSqlDatabase::database(name, true);
    }

    virtual QormSqlQueryBuilder::Ptr createSqlQueryBuilder() const override
    {
        return QormSqlQueryBuilder::Ptr(new QormSqlQueryBuilder());
    }

    void cleanup()
    {
        for (auto name : QSqlDatabase::connectionNames())
            QSqlDatabase::removeDatabase(name);
        QFile::remove(m_file);
    }

private:
    QString m_file;
    mutable bool m_isDatabaseInitialized;

    void initializeDatabase(QSqlDatabase& db) const
    {
        bool okOpen = db.open();
        Q_ASSERT(okOpen);

        QFile schemaFile(":/schema.sql");
        bool okOpenFile = schemaFile.open(QFile::ReadOnly);
        Q_ASSERT(okOpenFile);
        auto ddl = QString::fromLocal8Bit(schemaFile.readAll()).trimmed();
        auto ddlStatements = ddl.split(';', QString::SplitBehavior::SkipEmptyParts);

        QSqlQuery initialization(db);
        for (auto statement : ddlStatements)
            initialization.exec(statement.trimmed());
        db.close();
    }
};
