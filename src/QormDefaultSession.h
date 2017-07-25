#pragma once

#include <QMetaOrm/QormSession.h>
#include <QMetaOrm/QormLogger.h>

/**
* @todo - Add logging (TRACE/DEBUG)
*       - Added identity persistence
*       - Add object-pointer support (std::shared_ptr, QSharedPointer)
*       - Add selectOneByMapping (The template based variant actually does not support passing a custom mapping)
*/
class QMETAORM_LIBRARY_API QormDefaultSession : public QormSession {
public:
    QormDefaultSession(const QormDatabaseFactory::Ptr& databaseFactory, const QormLogger::Ptr& logger);

    virtual ~QormDefaultSession();

    SharedObject save(const SharedObject& entity, QormMetaEntity::Ptr mapping) override;

    void remove(const SharedObject& entity, QormMetaEntity::Ptr mapping) override;

    SharedObject selectOne(const QVariant& key, QormMetaEntity::Ptr mapping) override;

    QList<SharedObject> selectMany(QormMetaEntity::Ptr mapping, int skip, int pageSize) override;

    void selectMany(QormMetaEntity::Ptr mapping, ObjectHandler callback, int skip, int pageSize) override;

    void save(const QormSql& sqlQuery) override;

    void remove(const QormSql& sqlQuery) override;

    SharedObject selectOne(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping) override;

    QList<SharedObject> selectMany(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping) override;

    void selectMany(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping, ObjectHandler callback) override;

    void selectManyWithCustomMapping(const QormSql& sqlQuery,
                                     std::function<bool(const QormOnDemandRecordMapper*)> callback) override;

    void commit() override;

    void rollback() override;

private:
    QSharedPointer<QObject> create(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping);

    QSharedPointer<QObject> update(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping);

    void setupSession();

    QSqlDatabase m_database;
    QSharedPointer<class QormEntityMapper> m_entityMapper;
    QSharedPointer<class QormEntitySqlBuilder> m_entitySqlBuilder;
};
