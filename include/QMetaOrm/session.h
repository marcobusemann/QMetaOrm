#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/entitysqlbuilder.h>
#include <QMetaOrm/entitymapper.h>
#include <QMetaOrm/databasefactory.h>
#include <QMetaOrm/exceptions.h>

#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace QMetaOrm {

   /**
    * @brief The Session class
    * @todo - Add logging (TRACE/DEBUG)
    *       - Added identity persistence
    *       - Add custom sql support 
    *       - Add object-pointer support (std::shared_ptr, QSharedPointer)
    */
   class QMETAORM_LIBRARY_API Session
   {
   public:
      typedef QSharedPointer<Session> Ptr;

   public:
      Session(
         DatabaseFactory::Ptr databaseFactory,
         EntitySqlBuilder::Ptr entitySqlBuilder,
         EntityMapper::Ptr entityMapper,
         ConverterStore::Ptr converterStore);

      virtual ~Session();

      template <class T>
      T save(T entity, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      template <class T>
      void remove(T entity, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      template <class T, typename Key>
      T selectOne(Key key, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      template <class T>
      QList<T> selectMany(Criterion::Ptr criterion = Criterion::Ptr(), int skip = -1, int pageSize = -1, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      template <class T>
      void selectManyByCallback(Criterion::Ptr criterion, std::function<void(T)> callback, int skip = -1, int pageSize = -1, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      /*
      template <class T>
      void create(T entity, const QString &sql, MetaEntity mapping = MetaEntity());

      template <class T>
      void update(T entity, const QString &sql, MetaEntity mapping = MetaEntity());

      template <class T, typename Key>
      T selectOne(Key key, const QString &sql, MetaEntity mapping = MetaEntity());

      template <class T>
      QList<T> selectMany(const QString &sql, MetaEntity mapping = MetaEntity());
      */

      void commit();
      void rollback();

   private:

      template <class T>
      T create(T entity, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      template <class T>
      T update(T entity, MetaEntity::Ptr mapping = MetaEntity::Ptr());

      void setupSession();

      QSqlDatabase m_database;
      EntityMapper::Ptr m_entityMapper;
      EntitySqlBuilder::Ptr m_entitySqlBuilder;
      ConverterStore::Ptr m_converterStore;
   };

   //-----------------------------------------------------------------------------
   template <class T>
   T Session::save(T entity, MetaEntity::Ptr mapping) {
      setupSession();
      mapping = mapping != nullptr ? mapping : QMetaOrm::Mappings::mapping<T>();
      if (mapping->hasValidKey(entity))
         return update(entity, mapping);
      else
         return create(entity, mapping);
   }

   //-----------------------------------------------------------------------------
   template <class T>
   void Session::remove(T entity, MetaEntity::Ptr mapping) {
      setupSession();

      mapping = mapping != nullptr ? mapping : QMetaOrm::Mappings::mapping<T>();
      Q_ASSERT_X(mapping->hasValidKey(entity), "remove", "entity has no valid key, removing not possible.");

      QSqlQuery query(m_database);

      if (!query.prepare(m_entitySqlBuilder->buildRemove(mapping)))
         throw CouldNotPrepareQueryException(query.lastError());

      query.bindValue(0, mapping->getProperty(entity, mapping->getKeyProperty()));

      if (!query.exec())
         throw CouldNotExecuteQueryException(query.lastError());
   }

   //-----------------------------------------------------------------------------
   template <class T, typename Key>
   T Session::selectOne(Key key, MetaEntity::Ptr mapping) {
      setupSession();

      mapping = mapping != nullptr ? mapping : QMetaOrm::Mappings::mapping<T>();

      QSqlQuery query(m_database);

      if (!query.prepare(m_entitySqlBuilder->buildSelect(mapping)))
         throw CouldNotPrepareQueryException(query.lastError());

      query.bindValue(0, key);

      if (!query.exec())
         throw CouldNotExecuteQueryException(query.lastError());

      return query.next() ?
         m_entityMapper->mapToEntity<T>(mapping, query.record(), m_converterStore) :
         T();
   }

   //-----------------------------------------------------------------------------
   template <class T>
   QList<T> Session::selectMany(Criterion::Ptr criterion, int skip, int pageSize, MetaEntity::Ptr mapping) {
      QList<T> result;
      std::function<void(T)> func = [&result](T item) -> void {
         result.append(item);
      };
      selectManyByCallback(criterion, func, skip, pageSize, mapping);
      return result;
   }

   //-----------------------------------------------------------------------------
   template <class T>
   void Session::selectManyByCallback(Criterion::Ptr criterion, std::function<void(T)> callback, int skip, int pageSize, MetaEntity::Ptr mapping) {
      setupSession();

      mapping = mapping != nullptr ? mapping : QMetaOrm::Mappings::mapping<T>();

      QSqlQuery query(m_database);

      QVariantList conditions;
      QString sql = m_entitySqlBuilder->buildSelectMany(mapping, criterion, skip, pageSize, conditions);
      if (!query.prepare(sql))
         throw CouldNotPrepareQueryException(query.lastError());

      for(int i = 0; i < conditions.size(); i++)
         query.bindValue(i, conditions[i]);

      if (!query.exec())
         throw CouldNotExecuteQueryException(query.lastError());

      while (query.next())
         callback(m_entityMapper->mapToEntity<T>(mapping, query.record(), m_converterStore));
   }

   //-----------------------------------------------------------------------------
   template <class T>
   T Session::create(T entity, MetaEntity::Ptr mapping) {
      setupSession();

      mapping = mapping != nullptr ? mapping : QMetaOrm::Mappings::mapping<T>();

      QSqlQuery query(m_database);

      QStringList properties;
      qDebug() << m_entitySqlBuilder->buildInsert(mapping, properties);
      if (!query.prepare(m_entitySqlBuilder->buildInsert(mapping, properties)))
         throw CouldNotPrepareQueryException(query.lastError());

      for(int i = 0; i < properties.size(); i++)
         query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i], m_converterStore));

      if (!query.exec())
         throw CouldNotExecuteQueryException(query.lastError());

      if (query.first()) {
         mapping->setProperty(entity, mapping->getKeyProperty(), query.value(0));
         qDebug() << query.value(0);
      }

      return entity;
   }

   //-----------------------------------------------------------------------------
   template <class T>
   T Session::update(T entity, MetaEntity::Ptr mapping) {
      setupSession();

      mapping = mapping != nullptr ? mapping : QMetaOrm::Mappings::mapping<T>();

      QSqlQuery query(m_database);

      QStringList properties;

      if (!query.prepare(m_entitySqlBuilder->buildUpdate(mapping, properties)))
         throw CouldNotPrepareQueryException(query.lastError());

      for(int i = 0; i < properties.size(); i++)
         query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i], m_converterStore));
      query.bindValue(properties.size(), mapping->getProperty(entity, mapping->getKeyProperty()));

      if (!query.exec())
         throw CouldNotExecuteQueryException(query.lastError());

      return entity;
   }
   /*
   //-----------------------------------------------------------------------------
   template <class T>
   void Session::create(T entity, const QString &sql, MetaEntity mapping) {
      setupSession();

      mapping = mapping.isValid() ? mapping : QMetaOrm::Mappings::mapping<T>();

      QSqlQuery query(m_database);

      if (!query.prepare(sql))
         throw CouldNotPrepareQueryException(query.lastError());

      auto bindingParams = mapping.propertyMapping.keys()
      for(int i = 0; i < bindingParams.size(); i++)
      {
         auto bindingParam = bindingParams[i];
         auto value = mapping.propertyMapping[bindingParam];
         query.bindValue(bindingParam, mapping.getProperty(entity, value));
      }

      if (!query.exec())
         throw CouldNotExecuteQueryException(query.lastError());

      // Todo extend reverse mapping
      if (query.first() && !mapping.key.first.isEmpty())
         mapping.setProperty(entity, mapping.key.first, query.value(0));

      return entity;
   }

   //-----------------------------------------------------------------------------
   template <class T>
   void Session::update(T entity, const QString &sql, MetaEntity mapping) {

   }

   //-----------------------------------------------------------------------------
   template <class T, typename Key>
   T Session::selectOne(Key key, const QString &sql, MetaEntity mapping) {

   }

   //-----------------------------------------------------------------------------
   template <class T>
   QList<T> Session::selectMany(const QString &sql, MetaEntity mapping) {
      return QList<T>();
   }
   */
}