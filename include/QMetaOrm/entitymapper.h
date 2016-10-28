#pragma once

#include <QMetaOrm\converterstore.h>
#include <QMetaOrm\metaentity.h>
#include <QMetaOrm\exceptions.h>
#include <QMetaOrm\private.h>
#include <QMetaOrm\logger.h>

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
      EntityMapper(Logger::Ptr logger);

      template <class T>
      QSharedPointer<T> mapToEntity(MetaEntity::Ptr mapping, const QSqlRecord &record, ConverterStore::Ptr converterStore) {
         QSharedPointer<T> result(new T());
         mapToEntity(mapping, converterStore, m_prefixer.getRecordValuePrefixer(record), applyTo(result.objectCast<QObject>()));
         return result;
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

         auto entityFactory = mapping->getEntityFactory();
         auto entity = entityFactory->construct();

         mapToEntity(mapping, converterStore, getRecord, applyTo(entity));

         m_logger->trace(QString("Created reference for %1").arg(mapping->getSource()));

         return entityFactory->pack(entity);
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
   };
}
