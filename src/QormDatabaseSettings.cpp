#include <QMetaOrm/QormDatabaseSettings.h>
#include <QSqlDatabase>

const QString& QormDatabaseSettings::getDatabaseName() const
{
    return databaseName;
}

void QormDatabaseSettings::setDatabaseName(const QString& databaseName)
{
    this->databaseName = databaseName;
}

const QString& QormDatabaseSettings::getHostName() const
{
    return hostName;
}

void QormDatabaseSettings::setHostName(const QString& hostName)
{
    this->hostName = hostName;
}

int QormDatabaseSettings::getPort() const
{
    return port;
}

void QormDatabaseSettings::setPort(int port)
{
    this->port = port;
}

const QString& QormDatabaseSettings::getUserName() const
{
    return userName;
}

void QormDatabaseSettings::setUserName(const QString& userName)
{
    this->userName = userName;
}

const QString& QormDatabaseSettings::getPassword() const
{
    return password;
}

void QormDatabaseSettings::setPassword(const QString& password)
{
    this->password = password;
}

void QormDatabaseSettings::applyTo(QSqlDatabase* database) const
{
    if (!hostName.isEmpty())
        database->setHostName(hostName);

    if (port>0)
        database->setPort(port);

    if (!databaseName.isEmpty())
        database->setDatabaseName(databaseName);

    if (!userName.isEmpty())
        database->setUserName(userName);

    if (!password.isEmpty())
        database->setPassword(password);
}

QormDatabaseSettings::QormDatabaseSettings()
    :port(0)
{
}
