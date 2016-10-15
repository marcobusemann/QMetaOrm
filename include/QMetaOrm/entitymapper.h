#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/metaentity.h>
#include <QMetaOrm/converterstore.h>

#include <QSharedPointer>
#include <QSqlRecord>

namespace QMetaOrm {

   /**
     * 
     */
   class QMETAORM_LIBRARY_API EntityMapper
   {
   public:
      typedef QSharedPointer<EntityMapper> Ptr;

   public:
      template <class T>
      T mapToEntity(MetaEntity mapping, const QSqlRecord &record, ConverterStore::Ptr converterStore) {
         T result;

         auto resultValue = record.value(mapping.key.second);
         if (resultValue.isValid())
            mapping.setProperty(result, mapping.key.first, resultValue);

         auto keys = mapping.propertyMapping.keys();
         foreach(auto key, keys) {
            auto propertie = mapping.propertyMapping[key];
            auto resultValue = record.value(propertie.databaseName);
            if (resultValue.isValid()) {
               if (propertie.hasConverter()) {
                  auto converterName = propertie.converterName;
                  if (converterStore->hasConverter(converterName)) {
                     auto converter = converterStore->getConverterFor(converterName);
                     resultValue = converter->convert(resultValue);
                  }
                  else {
                     // TODO: introduce error handling
                     qDebug() << QString("Converter %1 not registered!").arg(converterName);
                  }
               }
               mapping.setProperty(result, key, resultValue);
            }
         }

         return result;
      }
   };
}