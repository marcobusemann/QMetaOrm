#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormEntityCacheFactory.h>
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

    void setEntityCacheFactory(const QormEntityCacheFactory::Ptr& cacheFactory);

    void setConverterStore(const QormConverterStore::Ptr& converterStore);

    virtual QormSession::Ptr createSession() const override;

private:
    QormDefaultSessionFactory(const QormDatabaseFactory::Ptr& databaseFactory);

    QormDatabaseFactory::Ptr databaseFactory;
    QormConverterStore::Ptr converterStore;
    QormLogger::Ptr logger;
    QormEntityCacheFactory::Ptr cacheFactory;
};
