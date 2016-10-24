#pragma once

#include <QMetaOrm/converterstore.h>
#include <QMetaOrm/metaentity.h>
#include <QMetaOrm\exceptions.h>
#include <QMetaOrm/private.h>

#include <QSharedPointer>
#include <QSqlRecord>
#include <QDebug>

#include <functional>

class QMETAORM_LIBRARY_API PropertyPrefixer {
public:
   typedef std::function<QVariant(const QString &)> Handler;

public:
   Handler getRecordValuePrefixer(const QSqlRecord &record, const QString &prefixString = QString());
   Handler getEmbeddedRecordValuePrefixer(Handler prefixer, const QString &prefix = QString());
   QString prepend(const QString &value, const QString &prefixString = QString());
};

namespace QMetaOrm {

   /**
     * 
     */
   class QMETAORM_LIBRARY_API EntityMapper {
   public:
      typedef QSharedPointer<EntityMapper> Ptr;
      typedef std::function<void(const QString &key, const QVariant &value)> ApplyHandler;

   public:
      template <class T>
      QSharedPointer<T> mapToEntity(MetaEntity::Ptr mapping, const QSqlRecord &record, ConverterStore::Ptr converterStore) {
         QSharedPointer<T> result(new T());
         mapToEntity(mapping, converterStore, m_prefixer.getRecordValuePrefixer(record), [&](const QString &prop, const QVariant &value) {
            result->setProperty(prop.toStdString().c_str(), value);
         });
         return result;
      }

      void mapToEntity(
         const MetaEntity::Ptr &mapping,
         const ConverterStore::Ptr &converterStore,
         PropertyPrefixer::Handler getRecord,
         ApplyHandler applyHandler) {

         mapKeyToEntity(mapping, getRecord, applyHandler);
         mapPropertiesToEntity(mapping, converterStore, getRecord, applyHandler);
      }

      void mapKeyToEntity(
         const MetaEntity::Ptr &mapping, 
         PropertyPrefixer::Handler getRecord,
         ApplyHandler applyHandler) {

         auto value = getRecord(mapping->getKeyDatabaseField());
         if (value.isValid())
            applyHandler(mapping->getKeyProperty(), value);
      }

      void mapPropertiesToEntity(
         const MetaEntity::Ptr &mapping,
         ConverterStore::Ptr converterStore,
         PropertyPrefixer::Handler getRecord,
         ApplyHandler applyHandler) {

         auto keys = mapping->getProperties();
         foreach(auto key, keys) {
            auto propertie = mapping->getProperty(key);
            QVariant value;
            if (propertie.isReference()) {
               value = createReference(propertie.reference, converterStore, m_prefixer.getEmbeddedRecordValuePrefixer(getRecord, propertie.databaseName));
            }
            else {
               value = getRecord(propertie.databaseName);
               if (value.isValid() && !value.isNull() && propertie.hasConverter())
                  value = applyConverter(propertie.converterName, value, converterStore);
            }
            applyHandler(key, value);
         }
      }

	  bool isValidObject(const MetaEntity::Ptr &mapping, PropertyPrefixer::Handler getRecord) {
		  QVariant key = getRecord(mapping->getKeyDatabaseField());
		  return key.isValid() && !key.isNull();
	  }

      QVariant createReference(const MetaEntity::Ptr &mapping, ConverterStore::Ptr converterStore, PropertyPrefixer::Handler getRecord) {
		 if (!isValidObject(mapping, getRecord))
			return QVariant();

		 auto newObject = mapping->createReferenceObject();
         if (newObject == nullptr)
            return QVariant();
         mapToEntity(mapping, converterStore, getRecord, [&newObject](const QString &prop, const QVariant &value) {
            newObject->setProperty(prop.toStdString().c_str(), value);
         });
         auto refCaster = mapping->getReferenceCaster();
         return refCaster == nullptr ? QVariant() : refCaster(newObject); // TODO: proper error handling
      }

      QVariant applyConverter(const QString &converterName, const QVariant &value, const ConverterStore::Ptr &store) {
         auto converter = store->getConverterFor(converterName);
         if (converter == nullptr)
            throw ConverterNotFoundException(converterName);
         return converter->convert(value);
      }

   private:
      PropertyPrefixer m_prefixer;
   };
}

Q_DECLARE_METATYPE(QSharedPointer<QObject>);
