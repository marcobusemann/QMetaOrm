#pragma once

#include <QMetaOrm/QormConverterStore.h>
#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormExceptions.h>
#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormLogger.h>
#include <QMetaOrm/QormCache.h>

#include <QSharedPointer>
#include <QSqlRecord>
#include <QDebug>

#include <functional>

class QMETAORM_LIBRARY_API QormPropertyPrefixer {
public:
   typedef std::function<QVariant(const QString &)> Handler;

public:
   Handler getRecordValuePrefixer(const QSqlRecord &record, const QString &prefixString = QString());
   Handler getEmbeddedRecordValuePrefixer(Handler prefixer, const QString &prefix = QString());
   QString prepend(const QString &value, const QString &prefixString = QString());
};

class QMETAORM_LIBRARY_API QormEntityMapper {
public:
   typedef QSharedPointer<QormEntityMapper> Ptr;
   typedef std::function<void(const QString &key, const QVariant &value)> ApplyHandler;

public:
   QormEntityMapper(
      const QormLogger::Ptr &logger,
      const QormEntityCache::Ptr &entityCache);

   QSharedPointer<QObject> mapToEntity(const QormMetaEntity::Ptr &mapping, const QSqlRecord &record, const QormConverterStore::Ptr &converterStore, const QString &prefix = QString()) {
      auto getRecordFunc = m_prefixer.getRecordValuePrefixer(record, prefix);
      QVariant result;
      if (isValidObject(mapping, getRecordFunc))
         result = createCachedReference(mapping, converterStore, getRecordFunc);
      else
         result = createReference(mapping, converterStore, getRecordFunc);
      return mapping->getEntityFactory()->unpack(result);
   }

   template <class T>
   QSharedPointer<T> mapToEntity(const QormMetaEntity::Ptr &mapping, const QSqlRecord &record, const QormConverterStore::Ptr &converterStore, const QString &prefix = QString()) {
      return mapToEntity(mapping, record, converterStore, prefix).objectCast<T>();
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
      const QormMetaEntity::Ptr &mapping,
      const QormConverterStore::Ptr &converterStore,
      QormPropertyPrefixer::Handler getRecord,
      ApplyHandler applyHandler) {

      mapKeyToEntity(mapping, getRecord, applyHandler);
      mapPropertiesToEntity(mapping, converterStore, getRecord, applyHandler);
   }

   void mapKeyToEntity(
      const QormMetaEntity::Ptr &mapping,
      QormPropertyPrefixer::Handler getRecord,
      ApplyHandler applyHandler) {

      auto value = getKeyFor(mapping, getRecord);
      if (value.isValid())
         applyHandler(mapping->getKeyProperty(), value);
   }

   QVariant getKeyFor(const QormMetaEntity::Ptr &mapping, QormPropertyPrefixer::Handler getRecord) {
      return getRecord(mapping->getKeyDatabaseField());
   }

   void mapPropertiesToEntity(
      const QormMetaEntity::Ptr &mapping,
      const QormConverterStore::Ptr &converterStore,
      QormPropertyPrefixer::Handler getRecord,
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
            if (propertie.hasConverter())
               value = applyConverter(propertie.converterName, value, converterStore);
         }
         applyHandler(key, value);
      }
   }

   bool isValidObject(const QormMetaEntity::Ptr &mapping, QormPropertyPrefixer::Handler getRecord) {
      QVariant key = getRecord(mapping->getKeyDatabaseField());
      return key.isValid() && !key.isNull();
   }

   QVariant createReference(const QormMetaEntity::Ptr &mapping, const QormConverterStore::Ptr &converterStore, QormPropertyPrefixer::Handler getRecord) {
      auto entityFactory = mapping->getEntityFactory();
      auto entity = entityFactory->construct();
      mapToEntity(mapping, converterStore, getRecord, applyTo(entity));
      return entityFactory->pack(entity);
   }

   QVariant createCachedReference(const QormMetaEntity::Ptr &mapping, const QormConverterStore::Ptr &converterStore, QormPropertyPrefixer::Handler getRecord) {
      auto entityFactory = mapping->getEntityFactory();
      auto key = getKeyFor(mapping, getRecord);
      if (!isValidObject(mapping, getRecord)) return QVariant();
      if (!m_entityCache->contains(key, mapping))
         m_entityCache->put(key, createReference(mapping, converterStore, getRecord), mapping);
      return m_entityCache->get(key, mapping);
   }

   QVariant applyConverter(const QString &converterName, const QVariant &value, const QormConverterStore::Ptr &store) {
      auto converter = store->getConverterFor(converterName);
      if (converter == nullptr)
         throw QormConverterNotFoundException(converterName);
      return converter->convert(value);
   }

private:
   QormPropertyPrefixer m_prefixer;
   QormLogger::Ptr m_logger;
   QormEntityCache::Ptr m_entityCache;
};
