#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/entitysqlbuilder.h>
#include <QMetaOrm/entitymapper.h>
#include <QMetaOrm/databasefactory.h>
#include <QMetaOrm/exceptions.h>

#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>

/**
 * @brief The Session class
 */
// TODO: - Add logging (TRACE/DEBUG)
class QMETAORM_LIBRARY_API Session
{
public:
   typedef QSharedPointer<Session> Ptr;

public:
   Session(
      DatabaseFactory::Ptr databaseFactory,
      EntitySqlBuilder::Ptr entitySqlBuilder,
      EntityMapper::Ptr entityMapper);

   virtual ~Session();

   template <class T>
   T save(T entity, MetaEntity mapping = MetaEntity());

   template <class T>
   void remove(T entity, MetaEntity mapping = MetaEntity());

   template <class T, typename Key>
   T selectOne(Key key, MetaEntity mapping = MetaEntity());

   template <class T>
   QList<T> selectMany(Criterion::Ptr criterion, int skip = -1, int pageSize = -1, MetaEntity mapping = MetaEntity());

   //void flush();

   void commit();
   void rollback();

private:

   // TODO:
   //  - Check if lastInsertId() works for the current driver.
   //  - Request sequence when lastInsertId() is not supported
   //  - another id generation mechanism?
   template <class T>
   T create(T entity, MetaEntity mapping = MetaEntity());

   template <class T>
   T update(T entity, MetaEntity mapping = MetaEntity());

   void setupSession();

   QSqlDatabase m_database;
   EntityMapper::Ptr m_entityMapper;
   EntitySqlBuilder::Ptr m_entitySqlBuilder;
};

//-----------------------------------------------------------------------------
template <class T>
T Session::save(T entity, MetaEntity mapping) {
   setupSession();
   mapping = mapping.isValid() ? mapping : QMetaOrmMappings::mapping<T>();
   if (mapping.hasValidKey(entity))
      return update(entity, mapping);
   else
      return create(entity, mapping);
}

//-----------------------------------------------------------------------------
template <class T>
void Session::remove(T entity, MetaEntity mapping) {
   setupSession();

   mapping = mapping.isValid() ? mapping : QMetaOrmMappings::mapping<T>();
   Q_ASSERT_X(mapping.hasValidKey(entity), "remove", "entity has no valid key, removing not possible.");

   QSqlQuery query(m_database);

   if (!query.prepare(m_entitySqlBuilder->buildRemove(mapping)))
      throw CouldNotPrepareQueryException(query.lastError());

   query.bindValue(0, mapping.getProperty(entity, mapping.key.first));

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());
}

//-----------------------------------------------------------------------------
template <class T, typename Key>
T Session::selectOne(Key key, MetaEntity mapping) {
   setupSession();

   mapping = mapping.isValid() ? mapping : QMetaOrmMappings::mapping<T>();

   QSqlQuery query(m_database);

   if (!query.prepare(m_entitySqlBuilder->buildSelect(mapping)))
      throw CouldNotPrepareQueryException(query.lastError());

   query.bindValue(0, key);

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   return query.next() ?
      m_entityMapper->mapToEntity<T>(mapping, query.record()) :
      T();
}

//-----------------------------------------------------------------------------
template <class T>
QList<T> Session::selectMany(Criterion::Ptr criterion, int skip, int pageSize, MetaEntity mapping) {
   setupSession();

   mapping = mapping.isValid() ? mapping : QMetaOrmMappings::mapping<T>();

   QSqlQuery query(m_database);

   QVariantList conditions;
   if (!query.prepare(m_entitySqlBuilder->buildSelectMany(mapping, criterion, skip, pageSize, conditions)))
      throw CouldNotPrepareQueryException(query.lastError());

   for(int i = 0; i < conditions.size(); i++)
      query.bindValue(i, conditions[i]);

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   QList<T> result;
   while (query.next()) {
      result.append(m_entityMapper->mapToEntity<T>(mapping, query.record()));
   }
   return result;
}

//-----------------------------------------------------------------------------
template <class T>
T Session::create(T entity, MetaEntity mapping) {
   setupSession();

   mapping = mapping.isValid() ? mapping : QMetaOrmMappings::mapping<T>();

   QSqlQuery query(m_database);

   QStringList properties;
   if (!query.prepare(m_entitySqlBuilder->buildInsert(mapping, properties)))
      throw CouldNotPrepareQueryException(query.lastError());

   for(int i = 0; i < properties.size(); i++)
      query.bindValue(i, mapping.getProperty(entity, properties[i]));

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   if (query.first())
      mapping.setProperty(entity, mapping.key.first, query.value(0));

   return entity;
}

//-----------------------------------------------------------------------------
template <class T>
T Session::update(T entity, MetaEntity mapping) {
   setupSession();

   mapping = mapping.isValid() ? mapping : QMetaOrmMappings::mapping<T>();

   QSqlQuery query(m_database);

   QStringList properties;

   if (!query.prepare(m_entitySqlBuilder->buildUpdate(mapping, properties)))
      throw CouldNotPrepareQueryException(query.lastError());

   for(int i = 0; i < properties.size(); i++)
      query.bindValue(i, mapping.getProperty(entity, properties[i]));
   query.bindValue(properties.size(), mapping.getProperty(entity, mapping.key.first));

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   return entity;
}
