#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/metaentity.h>

#include <QSharedPointer>
#include <QSqlRecord>

class QMETAORM_LIBRARY_API EntityMapper
{
public:
   typedef QSharedPointer<EntityMapper> Ptr;

public:
   template <class T>
   T mapToEntity(MetaEntity mapping, const QSqlRecord &record) {
      T result;

      auto resultValue = record.value(mapping.key.first);
      if (resultValue.isValid())
         mapping.setProperty(result, mapping.key.second, resultValue);

      auto keys = mapping.propertyMapping.keys();
      foreach(auto key, keys) {
         auto resultValue = record.value(mapping.propertyMapping[key]);
         if (resultValue.isValid())
            mapping.setProperty(result, key, resultValue);
      }

      return result;
   }
};
