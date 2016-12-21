#pragma once

#include <QMetaOrm/ConverterStore.h>
#include <QMetaOrm/MetaEntity.h>
#include <QMetaOrm/Exceptions.h>
#include <QMetaOrm/Private.h>
#include <QMetaOrm/Logger.h>
#include <QMetaOrm/Cache.h>

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
      EntityMapper(
         const Logger::Ptr &logger,
         const EntityCache::Ptr &entityCache);

      QSharedPointer<QObject> mapToEntity(const MetaEntity::Ptr &mapping, const QSqlRecord &record, const ConverterStore::Ptr &converterStore) {
         auto getRecordFunc = m_prefixer.getRecordValuePrefixer(record);
         QVariant result;
         if (isValidObject(mapping, getRecordFunc))
            result = createCachedReference(mapping, converterStore, getRecordFunc);
         else
            result = createReference(mapping, converterStore, getRecordFunc);
         return mapping->getEntityFactory()->unpack(result);
      }

      template <class T>
      QSharedPointer<T> mapToEntity(const MetaEntity::Ptr &mapping, const QSqlRecord &record, const ConverterStore::Ptr &converterStore) {
         return mapToEntity(mapping, record, converterStore).objectCast<T>();
      }

      ApplyHandler applyTo(QSharedPointer<QObject> &obj) {
         return [&](const QString &prop, const QVariant &value) {
            if (!obj->setProperty(prop.toStdString().c_str(), value)) {
               if (value.isValid()) {
                  qWarning() << "Could not apply " << value << " to " << prop;
                  qWarning() << "\tNote that the variants type and the type used in Q_PROPERTY must match exactly!";
                  qWarning() << "\tIf you use a typedef, be sure that Q_DECLARE_METATYPE is used for that type.";
               }
            }
         };
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

         auto value = getKeyFor(mapping, getRecord);
         if (value.isValid())
            applyHandler(mapping->getKeyProperty(), value);
      }

      QVariant getKeyFor(const MetaEntity::Ptr &mapping, PropertyPrefixer::Handler getRecord) {
         return getRecord(mapping->getKeyDatabaseField());
      }

      void mapPropertiesToEntity(
         const MetaEntity::Ptr &mapping,
         const ConverterStore::Ptr &converterStore,
         PropertyPrefixer::Handler getRecord,
         ApplyHandler applyHandler) {

         auto keys = mapping->getProperties();
         foreach(auto key, keys) {
            auto propertie = mapping->getProperty(key);
            QVariant value;
            if (propertie.isReference() && isValidObject(mapping, getRecord)) {
               value = createCachedReference(propertie.reference, converterStore, m_prefixer.getEmbeddedRecordValuePrefixer(getRecord, propertie.databaseName));
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

      QVariant createReference(const MetaEntity::Ptr &mapping, const ConverterStore::Ptr &converterStore, PropertyPrefixer::Handler getRecord) {
         auto entityFactory = mapping->getEntityFactory();
         auto entity = entityFactory->construct();
         mapToEntity(mapping, converterStore, getRecord, applyTo(entity));
         return entityFactory->pack(entity);
      }

      QVariant createCachedReference(const MetaEntity::Ptr &mapping, const ConverterStore::Ptr &converterStore, PropertyPrefixer::Handler getRecord) {
         auto entityFactory = mapping->getEntityFactory();
         auto key = getKeyFor(mapping, getRecord);

         if (!m_entityCache->contains(key, mapping))
            m_entityCache->put(key, createReference(mapping, converterStore, getRecord), mapping);

         return m_entityCache->get(key, mapping);
      }

      QVariant applyConverter(const QString &converterName, const QVariant &value, const ConverterStore::Ptr &store) {
         auto converter = store->getConverterFor(converterName);
         if (converter == nullptr)
            throw ConverterNotFoundException(converterName);
         return converter->convert(value);
      }

   private:
      PropertyPrefixer m_prefixer;
      Logger::Ptr m_logger;
      EntityCache::Ptr m_entityCache;
   };
}
