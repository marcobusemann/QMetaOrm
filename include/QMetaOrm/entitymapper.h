#pragma once

#include <QMetaOrm\converterstore.h>
#include <QMetaOrm\metaentity.h>
#include <QMetaOrm\exceptions.h>
#include <QMetaOrm\private.h>
#include <QMetaOrm\logger.h>
#include <QMetaOrm\cache.h>

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

      template <class T>
      QSharedPointer<T> mapToEntity(const MetaEntity::Ptr &mapping, const QSqlRecord &record, const ConverterStore::Ptr &converterStore) {
         auto getRecordFunc = m_prefixer.getRecordValuePrefixer(record);
         auto item = createReference(mapping, converterStore, getRecordFunc);
         return mapping->getEntityFactory()->unpack(item).objectCast<T>();
      }

      ApplyHandler applyTo(QSharedPointer<QObject> &obj) {
         return [&](const QString &prop, const QVariant &value) {
            if (!obj->setProperty(prop.toStdString().c_str(), value)) {
               if (value.isValid()) {
                  qWarning() << "Could not apply " << value << " to " << prop;
                  qWarning() << "\tNote that the variants type and the type used in Q_PROPERTY must match exactly!";
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
            if (propertie.isReference()) {
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
         if (!isValidObject(mapping, getRecord))
            return QVariant();
         
         auto entityFactory = mapping->getEntityFactory();
         auto entity = entityFactory->construct();
         mapToEntity(mapping, converterStore, getRecord, applyTo(entity));
         return entityFactory->pack(entity);
      }

      QVariant createCachedReference(const MetaEntity::Ptr &mapping, const ConverterStore::Ptr &converterStore, PropertyPrefixer::Handler getRecord) {
         if (!isValidObject(mapping, getRecord))
            return QVariant();

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
