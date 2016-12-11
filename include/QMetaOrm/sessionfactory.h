#pragma once

#include <QMetaOrm/DatabaseFactory.h>
#include <QMetaOrm/CacheFactory.h>
#include <QMetaOrm/Private.h>
#include <QMetaOrm/Session.h>
#include <QMetaOrm/Logger.h>

#include <QSharedPointer>

namespace QMetaOrm {

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
      virtual ConverterStore::Ptr getConverterStore() const = 0;
   };

   /**
    * @brief The DefaultSessionFactory class
    */
   class QMETAORM_LIBRARY_API DefaultSessionFactory : public SessionFactory
   {
   public:
      static SessionFactory::Ptr factory(
         const DatabaseFactory::Ptr &databaseFactory, 
         const Logger::Ptr &logger = Logger::Ptr(),
         const EntityCacheFactory::Ptr &cacheFactory = EntityCacheFactory::Ptr());

   public:
      DefaultSessionFactory(
         const DatabaseFactory::Ptr &databaseFactory, 
         const Logger::Ptr &logger,
         const EntityCacheFactory::Ptr &cacheFactory);
      virtual Session::Ptr createSession() const override;
      virtual ConverterStore::Ptr getConverterStore() const override;

   private:
      DatabaseFactory::Ptr m_databaseFactory;
      ConverterStore::Ptr m_converterStore;
      Logger::Ptr m_logger;
      EntityCacheFactory::Ptr m_cacheFactory;
   };
}