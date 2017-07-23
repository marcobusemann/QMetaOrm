#pragma once

#include <QMetaOrm/QormSessionFactory.h>
#include <QMetaOrm/QormLogger.h>

class QormDefaultSessionFactory : public QormSessionFactory {
public:
    static Ptr factory(const QormDatabaseFactory::Ptr& databaseFactory, const QormLogger::Ptr &logger);

public:
    virtual QormSession::Ptr createSession() const override;

private:
    QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory, const QormLogger::Ptr &logger);

    QormDatabaseFactory::Ptr databaseFactory;
    QormLogger::Ptr logger;
};