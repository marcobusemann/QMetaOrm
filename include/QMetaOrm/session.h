#pragma once

#include <QMetaOrm/Private.h>
#include <QMetaOrm/EntitySqlBuilder.h>
#include <QMetaOrm/EntityMapper.h>
#include <QMetaOrm/DatabaseFactory.h>
#include <QMetaOrm/Exceptions.h>
#include <QMetaOrm/Cache.h>

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
    *       - Add selectOneByMapping (The template based variant actually does not support passing a custom mapping)
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
      void save(QSharedPointer<T> &entity);
      void save(QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping);

	     template <class T>
      void remove(const QSharedPointer<T> &entity);
      void remove(const QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping);

      /**
        * See the template less variant for more information.
        * This one uses the default mapping for T (QMetaOrm::Mappings::mapping<T>()).
        */
      template <class T, typename Key>
      QSharedPointer<T> selectOne(Key key);

      /**
        * Returns one object from the database which matches the given key.
        * Data will be mapped according to the given mapping.
        * If the result object contains a key as specified in the mapping, the object will be cached per session.
        * \throws MoreThanOneResultException
        */
      QSharedPointer<QObject> selectOne(const QVariant &key, MetaEntity::Ptr mapping);

      /**
       * See the template less variant for more information.
       * This one uses the default mapping for T (QMetaOrm::Mappings::mapping<T>()).
       */
      template <class T>
      QSharedPointer<T> selectOneBySql(const QString &sql, const QVariantList &parameters = QVariantList());

      /**
       * Returns one object from the database which matches the given sql statement. 
       * Data will be mapped according to the given mapping.
       * If the result object contains a key as specified in the mapping, the object will be cached per session.
       * \throws MoreThanOneResultException
       */
      QSharedPointer<QObject> selectOneBySql(const QString &sql, MetaEntity::Ptr mapping, const QVariantList &parameters = QVariantList());

      template <class T>
      void selectManyByCallback(
         std::function<bool(const QSharedPointer<T> &)> callback, 
         Criterion::Ptr criterion = Criterion::Ptr(), 
         int skip = -1, 
         int pageSize = -1);
      void selectManyByCallback(
         MetaEntity::Ptr mapping,
         std::function<bool(const QSharedPointer<QObject> &)> callback,
         Criterion::Ptr criterion = Criterion::Ptr(), 
         int skip = -1, 
         int pageSize = -1);

      template <class T>
      void selectManyByCallbackBySql(
         const QString &sql, 
         std::function<bool(const QSharedPointer<T> &)> callback, 
         const QVariantList &parameters = QVariantList());
      void selectManyByCallbackBySql(
         const QString &sql, 
         MetaEntity::Ptr mapping,
         std::function<bool(const QSharedPointer<QObject> &)> callback,
         const QVariantList &parameters = QVariantList());

      /**
        * See the template less variant for more information.
        * This one uses the default mapping for T (QMetaOrm::Mappings::mapping<T>()).
        */
      template <class T>
      QList<QSharedPointer<T>> selectMany(Criterion::Ptr criterion = Criterion::Ptr(), int skip = -1, int pageSize = -1);

      /**
        * Returns multiple objects from the database matching the given criterion.
        * Data will be mapped according to the given mapping.
        * If the result object contains a key as specified in the mapping, the object will be cached per session.
        */
      QList<QSharedPointer<QObject>> selectMany(MetaEntity::Ptr mapping, Criterion::Ptr criterion = Criterion::Ptr(), int skip = -1, int pageSize = -1);


      /**
        * See the template less variant for more information.
        * This one uses the default mapping for T (QMetaOrm::Mappings::mapping<T>()).
        */
      template <class T>
      QList<QSharedPointer<T>> selectManyBySql(const QString &sql, const QVariantList &parameters = QVariantList());

      /**
        * Returns multiple objects from the database which matche the given sql statement.
        * Data will be mapped according to the given mapping.
        * If the result object contains a key as specified in the mapping, the object will be cached per session.
        */
      QList<QSharedPointer<QObject>> selectManyBySql(const QString &sql, MetaEntity::Ptr mapping, const QVariantList &parameters = QVariantList());

      void commit();
      void rollback();

   private:
      void create(QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping);
      void update(QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping);
      void setupSession();

      QSqlDatabase m_database;
      EntityMapper::Ptr m_entityMapper;
      EntitySqlBuilder::Ptr m_entitySqlBuilder;
      ConverterStore::Ptr m_converterStore;
   };

   template <class T>
   void Session::save(QSharedPointer<T> &entity) {
      auto generellEntity = entity.template objectCast<QObject>();
      save(generellEntity, QMetaOrm::Mappings::mapping<T>());
   }
   
   template <class T>
   void Session::remove(const QSharedPointer<T> &entity) {
      remove(entity, QMetaOrm::Mappings::mapping<T>());
   }

   template <class T, typename Key>
   QSharedPointer<T> Session::selectOne(Key key) {
      return selectOne(key, QMetaOrm::Mappings::mapping<T>()).template objectCast<T>();
   }

   template <class T>
   QSharedPointer<T> Session::selectOneBySql(const QString &sql, const QVariantList &parameters) {
      return selectOneBySql(sql, QMetaOrm::Mappings::mapping<T>(), parameters).template objectCast<T>();
   }

   template <class T>
   void Session::selectManyByCallback(
      std::function<bool(const QSharedPointer<T> &)> callback,
      Criterion::Ptr criterion,
      int skip,
      int pageSize) {
      selectManyByCallback(QMetaOrm::Mappings::mapping<T>(), callback, criterion, skip, pageSize);
   }

   template <class T>
   void Session::selectManyByCallbackBySql(
      const QString &sql,
      std::function<bool(const QSharedPointer<T> &)> callback,
      const QVariantList &parameters) {
      selectManyByCallbackBySql(sql, QMetaOrm::Mappings::mapping<T>(), callback, parameters);
   }

   template <class T>
   QList<QSharedPointer<T>> Session::selectMany(Criterion::Ptr criterion, int skip, int pageSize) {
      QList<QSharedPointer<T>> result;
      auto func = [&result](const QSharedPointer<QObject> &item) -> bool {
         result.append(item.objectCast<T>());
         return true;
      };
      selectManyByCallback(QMetaOrm::Mappings::mapping<T>(), func, criterion, skip, pageSize);
      return result;
   }

   template <class T>
   QList<QSharedPointer<T>> Session::selectManyBySql(const QString &sql, const QVariantList &parameters)
   {
      QList<QSharedPointer<T>> result;
      auto func = [&result](const QSharedPointer<QObject> &item) -> bool {
         result.append(item.objectCast<T>());
         return true;
      };
      selectManyByCallbackBySql(sql, QMetaOrm::Mappings::mapping<T>(), func, parameters);
      return result;
   }
}
