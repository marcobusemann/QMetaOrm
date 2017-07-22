#pragma once

#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormEntitySqlBuilder.h>
#include <QMetaOrm/QormEntityMapper.h>
#include <QMetaOrm/QormExceptions.h>
#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormEntityCache.h>

#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>

class QMETAORM_LIBRARY_API QOrmOnDemandRecordMapper {
public:
    virtual ~QOrmOnDemandRecordMapper() { }

    template<class T>
    QSharedPointer<T> mapToEntity(const QormMetaEntity::Ptr& mapping, const QString& prefix = QString()) const
    {
        return mapToEntity(mapping, prefix).objectCast<T>();
    }

    virtual QSharedPointer<QObject>
    mapToEntity(const QormMetaEntity::Ptr& mapping, const QString& prefix = QString()) const = 0;
};

/**
* @todo - Add logging (TRACE/DEBUG)
*       - Added identity persistence
*       - Add object-pointer support (std::shared_ptr, QSharedPointer)
*       - Add selectOneByMapping (The template based variant actually does not support passing a custom mapping)
*/
class QMETAORM_LIBRARY_API QormSession {
public:
    typedef QSharedPointer<QormSession> Ptr;

public:
    QormSession(
        QormDatabaseFactory::Ptr databaseFactory,
        QormEntitySqlBuilder::Ptr entitySqlBuilder,
        QormEntityMapper::Ptr entityMapper);

    virtual ~QormSession();

    /**
      * See the template less variant for more information.
      */
    template<class T>
    void save(QSharedPointer<T>& entity, QormMetaEntity::Ptr mapping);

    void saveBySql(const QString& sql, const QVariantList& parameters = QVariantList());

    /**
      * Creates or update (an id is present) the given entity using the given mapping.
      * If the entity is created, the new id will be applied.
      */
    void save(QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping);

    template<class T>
    void remove(const QSharedPointer<T>& entity, QormMetaEntity::Ptr mapping);

    void remove(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping);

    void removeBySql(const QString& sql, const QVariantList& parameters = QVariantList());

    /**
      * See the template less variant for more information.
      */
    template<class T, typename Key>
    QSharedPointer<T> selectOne(Key key, QormMetaEntity::Ptr mapping);

    /**
      * Returns one object from the database which matches the given key.
      * Data will be mapped according to the given mapping.
      * If the result object contains a key as specified in the mapping, the object will be cached per session.
      * \throws MoreThanOneResultException
      */
    QSharedPointer<QObject> selectOne(const QVariant& key, QormMetaEntity::Ptr mapping);

    QVariantList selectOneBySql(const QString& sql, const QVariantList& parameters = QVariantList());

    /**
      * See the template less variant for more information.
      */
    template<class T>
    QSharedPointer<T>
    selectOneBySql(const QString& sql, QormMetaEntity::Ptr mapping, const QVariantList& parameters = QVariantList());

    /**
      * Returns one object from the database which matches the given sql statement.
      * Data will be mapped according to the given mapping.
      * If the result object contains a key as specified in the mapping, the object will be cached per session.
      * \throws MoreThanOneResultException
      */
    QSharedPointer<QObject>
    selectOneBySql(const QString& sql, QormMetaEntity::Ptr mapping, const QVariantList& parameters = QVariantList());

    template<class T>
    void selectManyByCallback(
        QormMetaEntity::Ptr mapping,
        std::function<bool(const QSharedPointer<T>&)> callback,
        int skip = -1,
        int pageSize = -1);

    void selectManyByCallback(
        QormMetaEntity::Ptr mapping,
        std::function<bool(const QSharedPointer<QObject>&)> callback,
        int skip = -1,
        int pageSize = -1);

    template<class T>
    void selectManyByCallbackBySql(
        QormMetaEntity::Ptr mapping,
        const QString& sql,
        std::function<bool(const QSharedPointer<T>&)> callback,
        const QVariantList& parameters = QVariantList());

    void selectManyByCallbackBySql(
        const QString& sql,
        QormMetaEntity::Ptr mapping,
        std::function<bool(const QSharedPointer<QObject>&)> callback,
        const QVariantList& parameters = QVariantList());

    /**
      * See the template less variant for more information.
      */
    template<class T>
    QList<QSharedPointer<T>> selectMany(QormMetaEntity::Ptr mapping, int skip = -1, int pageSize = -1);

    /**
      * Returns multiple objects from the database matching the given criterion.
      * Data will be mapped according to the given mapping.
      * If the result object contains a key as specified in the mapping, the object will be cached per session.
      */
    QList<QSharedPointer<QObject>> selectMany(QormMetaEntity::Ptr mapping, int skip = -1, int pageSize = -1);

    /**
      * See the template less variant for more information.
      */
    template<class T>
    QList<QSharedPointer<T>>
    selectManyBySql(const QString& sql, QormMetaEntity::Ptr mapping, const QVariantList& parameters = QVariantList());

    /**
      * Returns multiple objects from the database which matche the given sql statement.
      * Data will be mapped according to the given mapping.
      * If the result object contains a key as specified in the mapping, the object will be cached per session.
      */
    QList<QSharedPointer<QObject>>
    selectManyBySql(const QString& sql, QormMetaEntity::Ptr mapping, const QVariantList& parameters = QVariantList());

    void selectManyBySqlWithCustomMapping(
        const QString& sql,
        std::function<bool(const QOrmOnDemandRecordMapper*)> callback,
        const QVariantList& parameters = QVariantList());

    void commit();

    void rollback();

private:
    void create(QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping);

    void update(QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping);

    void setupSession();

    QSqlDatabase m_database;
    QormEntityMapper::Ptr m_entityMapper;
    QormEntitySqlBuilder::Ptr m_entitySqlBuilder;
};

template<class T>
void QormSession::save(QSharedPointer<T>& entity, QormMetaEntity::Ptr mapping)
{
    auto generellEntity = entity.template objectCast<QObject>();
    save(generellEntity, mapping);
}

template<class T>
void QormSession::remove(const QSharedPointer<T>& entity, QormMetaEntity::Ptr mapping)
{
    auto generellEntity = entity.template objectCast<QObject>();
    remove(generellEntity, mapping);
}

template<class T, typename Key>
QSharedPointer<T> QormSession::selectOne(Key key, QormMetaEntity::Ptr mapping)
{
    return selectOne(key, mapping).template objectCast<T>();
}

template<class T>
QSharedPointer<T>
QormSession::selectOneBySql(const QString& sql, QormMetaEntity::Ptr mapping, const QVariantList& parameters)
{
    return selectOneBySql(sql, mapping, parameters).template objectCast<T>();
}

template<class T>
void QormSession::selectManyByCallback(
    QormMetaEntity::Ptr mapping,
    std::function<bool(const QSharedPointer<T>&)> callback,
    int skip,
    int pageSize)
{
    selectManyByCallback(mapping, [callback](const QSharedPointer<QObject>& item) -> bool {
        return callback(item.objectCast<T>());
    }, skip, pageSize);
}

template<class T>
void QormSession::selectManyByCallbackBySql(
    QormMetaEntity::Ptr mapping,
    const QString& sql,
    std::function<bool(const QSharedPointer<T>&)> callback,
    const QVariantList& parameters)
{
    selectManyByCallbackBySql(sql, mapping, [callback](const QSharedPointer<QObject>& item) -> bool {
        return callback(item.objectCast<T>());
    }, parameters);
}

template<class T>
QList<QSharedPointer<T>> QormSession::selectMany(QormMetaEntity::Ptr mapping, int skip, int pageSize)
{
    QList<QSharedPointer<T>> result;
    auto func = [&result](const QSharedPointer<QObject>& item) -> bool {
        result.append(item.objectCast<T>());
        return true;
    };
    selectManyByCallback(mapping, func, skip, pageSize);
    return result;
}

template<class T>
QList<QSharedPointer<T>>
QormSession::selectManyBySql(const QString& sql, QormMetaEntity::Ptr mapping, const QVariantList& parameters)
{
    QList<QSharedPointer<T>> result;
    auto func = [&result](const QSharedPointer<QObject>& item) -> bool {
        result.append(item.objectCast<T>());
        return true;
    };
    selectManyByCallbackBySql(sql, mapping, func, parameters);
    return result;
}
