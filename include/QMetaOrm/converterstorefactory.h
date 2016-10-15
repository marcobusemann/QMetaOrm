#pragma once

#include <QMetaOrm\converterstore.h>
#include <QMetaOrm/private.h>
#include <qsharedpointer.h>

namespace QMetaOrm {

   /**
   * @brief The ConverterStoreFactory class
   */
   class QMETAORM_LIBRARY_API ConverterStoreFactory
   {
   public:
      typedef QSharedPointer<ConverterStoreFactory> Ptr;

   public:
      virtual ~ConverterStoreFactory() {}
      virtual ConverterStore::Ptr createConverterStore() const = 0;
   };

   /**
   * @brief The DefaultConverterStoreFactory class
   */
   class QMETAORM_LIBRARY_API DefaultConverterStoreFactory : public ConverterStoreFactory
   {
   public:
      typedef QSharedPointer<DefaultConverterStoreFactory> Ptr;
      static DefaultConverterStoreFactory::Ptr factory();

   public:
      DefaultConverterStoreFactory();
      virtual ConverterStore::Ptr createConverterStore() const override;

   private:
      ConverterStore::Ptr m_store;
   };
}