#pragma once

#include <QMetaOrm/QormPrivate.h>
#include <QString>

class QMETAORM_LIBRARY_API QormDatabaseSettings {
public:
    QormDatabaseSettings();

    const QString& getDatabaseName() const;

    void setDatabaseName(const QString& databaseName);

    const QString& getHostName() const;

    void setHostName(const QString& hostName);

    int getPort() const;

    void setPort(int port);

    const QString& getUserName() const;

    void setUserName(const QString& userName);

    const QString& getPassword() const;

    void setPassword(const QString& password);

    void applyTo(class QSqlDatabase* database) const;

private:
    QString databaseName;
    QString hostName;
    int port;
    QString userName;
    QString password;
};
