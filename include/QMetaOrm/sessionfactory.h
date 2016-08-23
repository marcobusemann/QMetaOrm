#pragma once

#include <QMetaOrm/databasefactory.h>
#include <QMetaOrm/private.h>
#include <QMetaOrm/session.h>

#include <QSharedPointer>

/**
 * @brief The SessionFactory class
 */
class QMETAORM_LIBRARY_API SessionFactory
{
public:
   typedef QSharedPointer<SessionFactory> Ptr;

public:
   virtual ~SessionFactory() {}
   virtual Session::Ptr createSession() const = 0;
};

/**
 * @brief The DefaultSessionFactory class
 */
class QMETAORM_LIBRARY_API DefaultSessionFactory: public SessionFactory
{
public:
   static SessionFactory::Ptr create(DatabaseFactory::Ptr databaseFactory);

public:
   DefaultSessionFactory(DatabaseFactory::Ptr databaseFactory);
   virtual Session::Ptr createSession() const override;

private:
   DatabaseFactory::Ptr m_databaseFactory;
};
