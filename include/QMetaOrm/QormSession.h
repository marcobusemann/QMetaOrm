#pragma once

#include <QMetaOrm/QormOnDemandRecordMapper.h>
#include <QMetaOrm/QormDatabaseFactory.h>
#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormSql.h>

#include <QSharedPointer>

class QMETAORM_LIBRARY_API QormSession {
public:
    typedef QSharedPointer<QormSession> Ptr;
    typedef std::function<bool(const QSharedPointer<QObject>&)> ObjectHandler;
    typedef QSharedPointer<QObject> SharedObject;

public:
    virtual ~QormSession() { };

    virtual SharedObject save(const SharedObject& entity, QormMetaEntity::Ptr mapping) = 0;

    virtual void remove(const SharedObject& entity, QormMetaEntity::Ptr mapping) = 0;

    virtual SharedObject selectOne(const QVariant& key, QormMetaEntity::Ptr mapping) = 0;

    virtual QList<SharedObject> selectMany(QormMetaEntity::Ptr mapping, int skip = -1, int pageSize = -1) = 0;

    virtual void selectMany(QormMetaEntity::Ptr mapping, ObjectHandler callback, int skip = -1, int pageSize = -1) = 0;

    virtual void save(const QormSql& sqlQuery) = 0;

    virtual void remove(const QormSql& sqlQuery) = 0;

    virtual SharedObject selectOne(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping) = 0;

    virtual QList<SharedObject> selectMany(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping) = 0;

    virtual void selectMany(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping, ObjectHandler callback) = 0;

    virtual void selectManyWithCustomMapping(const QormSql& sqlQuery,
                                             std::function<bool(const QormOnDemandRecordMapper*)> callback) = 0;

    virtual void commit() = 0;

    virtual void rollback() = 0;

    template<class T>
    QSharedPointer<T> save(const QSharedPointer<T>& entity, QormMetaEntity::Ptr mapping)
    {
        auto generalEntity = entity.template objectCast<QObject>();
        return save(generalEntity, mapping).template objectCast<T>();
    }

    template<class T>
    void remove(const QSharedPointer<T>& entity, QormMetaEntity::Ptr mapping)
    {
        auto generalEntity = entity.template objectCast<QObject>();
        remove(generalEntity, mapping);
    }

    template<class T>
    QSharedPointer<T> selectOne(const QVariant& key, QormMetaEntity::Ptr mapping)
    {
        return selectOne(key, mapping).template objectCast<T>();
    };

    template<class T>
    QSharedPointer<T> selectOne(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping)
    {
        return selectOne(sqlQuery, mapping).template objectCast<T>();
    }

    template<class T>
    void selectMany(
        QormMetaEntity::Ptr mapping,
        std::function<bool(const QSharedPointer<T>&)> callback,
        int skip = -1,
        int pageSize = -1)
    {
        selectMany(mapping, [callback](const QSharedPointer<QObject>& item) -> bool {
            return callback(item.template objectCast<T>());
        }, skip, pageSize);
    }

    template<class T>
    void selectMany(
        const QormSql& sqlQuery,
        QormMetaEntity::Ptr mapping,
        std::function<bool(const QSharedPointer<T>&)> callback)
    {
        selectMany(sqlQuery, mapping, [callback](const QSharedPointer<QObject>& item) -> bool {
            return callback(item.template objectCast<T>());
        });
    }

    template<class T>
    QList<QSharedPointer<T>> selectMany(QormMetaEntity::Ptr mapping, int skip = -1, int pageSize = -1)
    {
        QList<QSharedPointer<T>> result;
        auto func = [&result](const QSharedPointer<QObject>& item) -> bool {
            result.append(item.objectCast<T>());
            return true;
        };
        selectMany(mapping, func, skip, pageSize);
        return result;
    }

    template<class T>
    QList<QSharedPointer<T>> selectMany(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping)
    {
        QList<QSharedPointer<T>> result;
        auto func = [&result](const QSharedPointer<QObject>& item) -> bool {
            result.append(item.objectCast<T>());
            return true;
        };
        selectMany(sqlQuery, mapping, func);
        return result;
    }
};
