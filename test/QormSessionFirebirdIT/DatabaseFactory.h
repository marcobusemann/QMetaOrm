#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormFirebirdSqlQueryBuilder.h>

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include <QFile>
#include <QDebug>

namespace
{
    const auto TEMP_DATABASE_FILE = QDir::currentPath() + "/temp.fdb";
}

class FirebirdDatabaseFactory : public QormDatabaseFactory
{
public:
    static Ptr factory()
    {
        return Ptr(new FirebirdDatabaseFactory());
    }

public:
    FirebirdDatabaseFactory()
        : m_isDatabaseInitialized(false)
    {
    }

    ~FirebirdDatabaseFactory()
    {
        cleanup();
    }

    virtual QSqlDatabase createDatabase(const QString& name = QString()) const override
    {
        if (!QSqlDatabase::contains(name))
        {
            QFile::remove(TEMP_DATABASE_FILE);
            QFile::copy(":/EmptyFirebirdDatabase.fdb", TEMP_DATABASE_FILE);
            QFile(TEMP_DATABASE_FILE).setPermissions(QFile::Permission::ReadUser | QFile::Permission::WriteUser);

            QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE", name);
            db.setDatabaseName(TEMP_DATABASE_FILE);
            db.setHostName("localhost/3052");
            db.setUserName("SYSDBA");
            db.setPassword("MASTERKEY");

            if (!m_isDatabaseInitialized)
            {
                initializeDatabase(db);
                m_isDatabaseInitialized = true;
            }
        }
        return QSqlDatabase::database(name, true);
    }

    virtual QormSqlQueryBuilder::Ptr createSqlQueryBuilder() const override
    {
        return QormSqlQueryBuilder::Ptr(new QormFirebirdSqlQueryBuilder());
    }

    void cleanup()
    {
        for (auto name : QSqlDatabase::connectionNames())
            QSqlDatabase::removeDatabase(name);
        QFile::remove(TEMP_DATABASE_FILE);
    }

private:
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
