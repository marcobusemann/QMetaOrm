#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormSession.h>
#include <QMetaOrm/QormLogger.h>

#include <QSharedPointer>

class QMETAORM_LIBRARY_API QormSessionFactory {
public:
    typedef QSharedPointer<QormSessionFactory> Ptr;

public:
    virtual ~QormSessionFactory() { }

    virtual QormSession::Ptr createSession() const = 0;
};

class QMETAORM_LIBRARY_API QormDefaultSessionFactory : public QormSessionFactory {
public:
    typedef QSharedPointer<QormDefaultSessionFactory> Ptr;

    static Ptr factory(const QormDatabaseFactory::Ptr& databaseFactory);

public:
    void setLogger(const QormLogger::Ptr& logger);

    virtual QormSession::Ptr createSession() const override;

private:
    QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory);

    QormDatabaseFactory::Ptr databaseFactory;
    QormLogger::Ptr logger;
};
