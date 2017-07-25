#include <QMetaOrm/QormExceptions.h>

#include "QormEntitySqlBuilder.h"
#include "QormDefaultSession.h"
#include "QormEntityMapper.h"

#include <QSqlQuery>
#include <QUuid>

class QormOnDemandRecordMapperImpl : public QormOnDemandRecordMapper {
public:
    QormOnDemandRecordMapperImpl(
        std::function<QSharedPointer<QObject>(const QormMetaEntity::Ptr&, const QString&)> callback)
        :m_callback(callback)
    {
    }

    virtual QSharedPointer<QObject>
    mapToEntity(const QormMetaEntity::Ptr& mapping, const QString& prefix) const override
    {
        return m_callback(mapping, prefix);
    }

private:
    std::function<QSharedPointer<QObject>(const QormMetaEntity::Ptr&, const QString&)> m_callback;
};

QString GetThreadIdentifier()
{
    return QUuid::createUuid().toString();
}

QormDefaultSession::QormDefaultSession(const QormDatabaseFactory::Ptr &databaseFactory, const QormLogger::Ptr& logger)
    :m_database(databaseFactory->createDatabase(GetThreadIdentifier()))
     , m_entityMapper(QormEntityMapper::Ptr(new QormEntityMapper(logger)))
     , m_entitySqlBuilder(QormEntitySqlBuilder::Ptr(new QormEntitySqlBuilder()))
{
}

QormDefaultSession::~QormDefaultSession()
{
    rollback();
}

void QormDefaultSession::commit()
{
    m_database.commit();
}

void QormDefaultSession::rollback()
{
    m_database.rollback();
}

void QormDefaultSession::setupSession()
{
    if (!m_database.isOpen() && !m_database.open())
        throw QormConnectToDatabaseException(m_database.lastError());

    m_database.transaction();
}

QSharedPointer<QObject> QormDefaultSession::save(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping)
{
    setupSession();
    return mapping->hasValidKey(entity) ?
           update(entity, mapping) :
           create(entity, mapping);
}

void QormDefaultSession::save(const QormSql& sqlQuery)
{
    remove(sqlQuery);
}

void QormDefaultSession::remove(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping)
{
    setupSession();
    Q_ASSERT_X(mapping->hasValidKey(entity), "remove", "entity has no valid key, removing not possible.");

    QSqlQuery query(m_database);

    if (!query.prepare(m_entitySqlBuilder->buildRemove(mapping)))
        throw QormCouldNotPrepareQueryException(query.lastError());

    query.bindValue(0, mapping->getProperty(entity, mapping->getKeyProperty()));

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());
}

void QormDefaultSession::remove(const QormSql& sqlQuery)
{
    setupSession();

    QSqlQuery query(m_database);

    if (!query.prepare(sqlQuery.sql))
        throw QormCouldNotPrepareQueryException(query.lastError());

    auto parameters = sqlQuery.parameters;
    for (int i = 0; i<parameters.size(); i++)
        query.bindValue(i, parameters[i]);

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());
}

QSharedPointer<QObject> QormDefaultSession::create(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping)
{
    setupSession();

    QSqlQuery query(m_database);
    auto result = entity;

    QStringList properties;
    auto keyStrategy = mapping->getKeyGenerationStrategy();

    if (keyStrategy==KeyGenerationStrategy::Sequence) {
        QSqlQuery keyQuery(m_database);
        if (!keyQuery.exec(m_entitySqlBuilder->buildSequenceSelect(mapping)))
            throw QormCouldNotQueryNextSequenceValueException(query.lastError());

        if (!keyQuery.first())
            throw QormCouldNotQueryNextSequenceValueException(query.lastError());

        mapping->setProperty(result, mapping->getKeyProperty(), keyQuery.value(0));

        if (!query.prepare(m_entitySqlBuilder->buildInsertForSequence(mapping, properties)))
            throw QormCouldNotPrepareQueryException(query.lastError());
    }
    else if (keyStrategy==KeyGenerationStrategy::Identity) {
        if (!query.prepare(m_entitySqlBuilder->buildInsertForIdentity(mapping, properties)))
            throw QormCouldNotPrepareQueryException(query.lastError());
    }
    else
        Q_ASSERT(false);

    for (int i = 0; i<properties.size(); i++)
        query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i]));

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());

    if (keyStrategy==KeyGenerationStrategy::Identity)
        mapping->setProperty(result, mapping->getKeyProperty(), query.lastInsertId());

    if (query.first())
        mapping->setProperty(result, mapping->getKeyProperty(), query.value(0));
    return result;
}

QSharedPointer<QObject> QormDefaultSession::update(const QSharedPointer<QObject>& entity, QormMetaEntity::Ptr mapping)
{
    setupSession();

    QSqlQuery query(m_database);

    QStringList properties;

    if (!query.prepare(m_entitySqlBuilder->buildUpdate(mapping, properties)))
        throw QormCouldNotPrepareQueryException(query.lastError());

    for (int i = 0; i<properties.size(); i++)
        query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i]));
    query.bindValue(properties.size(), mapping->getProperty(entity, mapping->getKeyProperty()));

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());

    return entity;
}

QSharedPointer<QObject> QormDefaultSession::selectOne(const QVariant& key, QormMetaEntity::Ptr mapping)
{
    return selectOne(QormSql(m_entitySqlBuilder->buildSelect(mapping), QVariantList() << key), mapping);
}

QSharedPointer<QObject> QormDefaultSession::selectOne(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping)
{
    setupSession();

    QSqlQuery query(m_database);

    if (!query.prepare(sqlQuery.sql))
        throw QormCouldNotPrepareQueryException(query.lastError());

    auto parameters = sqlQuery.parameters;
    for (int i = 0; i<parameters.size(); i++)
        query.bindValue(i, parameters[i]);

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());

    auto result = query.next() ?
                  m_entityMapper->mapToEntity(mapping, query.record()) :
                  QSharedPointer<QObject>();

    if (query.next())
        throw QormMoreThanOneResultException();

    return result;
}

void QormDefaultSession::selectMany(
    QormMetaEntity::Ptr mapping,
    std::function<bool(const QSharedPointer<QObject>&)> callback,
    int skip,
    int pageSize)
{
    QVariantList conditions;
    auto sql = m_entitySqlBuilder->buildSelectMany(mapping, skip, pageSize, conditions);
    selectMany(
        QormSql(sql, conditions),
        mapping,
        callback);
}

void QormDefaultSession::selectMany(
    const QormSql& sqlQuery,
    QormMetaEntity::Ptr mapping,
    std::function<bool(const QSharedPointer<QObject>&)> callback)
{
    setupSession();

    QSqlQuery query(m_database);

    if (!query.prepare(sqlQuery.sql))
        throw QormCouldNotPrepareQueryException(query.lastError());

    auto parameters = sqlQuery.parameters;
    for (int i = 0; i<parameters.size(); i++)
        query.bindValue(i, parameters[i]);

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());

    bool continueWork = true;
    while (query.next() && continueWork)
        continueWork = callback(m_entityMapper->mapToEntity(mapping, query.record()));
}

QList<QSharedPointer<QObject>> QormDefaultSession::selectMany(QormMetaEntity::Ptr mapping, int skip, int pageSize)
{
    QList<QSharedPointer<QObject>> result;
    auto func = [&result](const QSharedPointer<QObject>& item) -> bool {
        result.append(item);
        return true;
    };
    selectMany(mapping, func, skip, pageSize);
    return result;
}

QList<QSharedPointer<QObject>>
QormDefaultSession::selectMany(const QormSql& sqlQuery, QormMetaEntity::Ptr mapping)
{
    QList<QSharedPointer<QObject>> result;
    auto func = [&result](const QSharedPointer<QObject>& item) -> bool {
        result.append(item);
        return true;
    };
    selectMany(sqlQuery, mapping, func);
    return result;
}

void QormDefaultSession::selectManyWithCustomMapping(
    const QormSql& sqlQuery,
    std::function<bool(const QormOnDemandRecordMapper*)> callback)
{
    setupSession();

    QSqlQuery query(m_database);

    if (!query.prepare(sqlQuery.sql))
        throw QormCouldNotPrepareQueryException(query.lastError());

    auto parameters = sqlQuery.parameters;
    for (int i = 0; i<parameters.size(); i++)
        query.bindValue(i, parameters[i]);

    if (!query.exec())
        throw QormCouldNotExecuteQueryException(query.lastError());

    QormOnDemandRecordMapperImpl recordMapper([&](const QormMetaEntity::Ptr& mapping, const QString& prefix) {
        return m_entityMapper->mapToEntity(mapping, query.record(), prefix);
    });

    bool continueWork = true;
    while (query.next() && continueWork)
        continueWork = callback(&recordMapper);
}
