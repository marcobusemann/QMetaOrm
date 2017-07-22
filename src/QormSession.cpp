#include <QMetaOrm/QormSession.h>
#include <QMetaOrm/QormExceptions.h>
#include <QUuid>

class QOrmOnDemandRecordMapperImpl : public QOrmOnDemandRecordMapper
{
public:
   QOrmOnDemandRecordMapperImpl(std::function<QSharedPointer<QObject>(const QormMetaEntity::Ptr&, const QString&)> callback)
      : m_callback(callback)
   {
   }

   virtual QSharedPointer<QObject> mapToEntity(const QormMetaEntity::Ptr& mapping, const QString& prefix) const override
   {
      return m_callback(mapping, prefix);
   }

private:
   std::function<QSharedPointer<QObject>(const QormMetaEntity::Ptr&, const QString&)> m_callback;
};

QString GetThreadIdentifier() {
   return QUuid::createUuid().toString();
}

QormSession::QormSession(
   QormDatabaseFactory::Ptr databaseFactory,
   QormEntitySqlBuilder::Ptr entitySqlBuilder,
   QormEntityMapper::Ptr entityMapper,
   QormConverterStore::Ptr converterStore)
   : m_database(databaseFactory->createDatabase(GetThreadIdentifier()))
   , m_entityMapper(entityMapper)
   , m_entitySqlBuilder(entitySqlBuilder)
   , m_converterStore(converterStore)
{
}

QormSession::~QormSession() {
   rollback();
}

void QormSession::commit() {
   m_database.commit();
}

void QormSession::rollback() {
   m_database.rollback();
}

void QormSession::setupSession() {
   if (!m_database.isOpen() && !m_database.open())
      throw QormConnectToDatabaseException(m_database.lastError());

   m_database.transaction();
}

void QormSession::save(QSharedPointer<QObject> &entity, QormMetaEntity::Ptr mapping)
{
   setupSession();
   if (mapping->hasValidKey(entity))
      return update(entity, mapping);
   else
      return create(entity, mapping);
}

void QormSession::saveBySql(const QString &sql, const QVariantList &parameters)
{
   removeBySql(sql, parameters);
}

void QormSession::remove(const QSharedPointer<QObject> &entity, QormMetaEntity::Ptr mapping)
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

void QormSession::removeBySql(const QString &sql, const QVariantList &parameters)
{
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw QormCouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());
}

void QormSession::create(QSharedPointer<QObject> &entity, QormMetaEntity::Ptr mapping) {
   setupSession();

   QSqlQuery query(m_database);

   QStringList properties;
   auto keyStrategy = mapping->getKeyGenerationStrategy();

   if (keyStrategy == KeyGenerationStrategy::Sequence) {
      QSqlQuery keyQuery(m_database);
      if (!keyQuery.exec(m_entitySqlBuilder->buildSequenceSelect(mapping)))
         throw QormCouldNotQueryNextSequenceValueException(query.lastError());

      if (!keyQuery.first())
         throw QormCouldNotQueryNextSequenceValueException(query.lastError());

      mapping->setProperty(entity, mapping->getKeyProperty(), keyQuery.value(0));

      if (!query.prepare(m_entitySqlBuilder->buildInsertForSequence(mapping, properties)))
         throw QormCouldNotPrepareQueryException(query.lastError());
   }
   else if (keyStrategy == KeyGenerationStrategy::Identity) {
      if (!query.prepare(m_entitySqlBuilder->buildInsertForIdentity(mapping, properties)))
         throw QormCouldNotPrepareQueryException(query.lastError());
   }
   else
      Q_ASSERT(false);

   for (int i = 0; i < properties.size(); i++)
      query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i], m_converterStore));

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());

   if (keyStrategy == KeyGenerationStrategy::Identity)
      mapping->setProperty(entity, mapping->getKeyProperty(), query.lastInsertId());

   if (query.first())
      mapping->setProperty(entity, mapping->getKeyProperty(), query.value(0));
}

void QormSession::update(QSharedPointer<QObject> &entity, QormMetaEntity::Ptr mapping) {
   setupSession();

   QSqlQuery query(m_database);

   QStringList properties;

   if (!query.prepare(m_entitySqlBuilder->buildUpdate(mapping, properties)))
      throw QormCouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < properties.size(); i++)
      query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i], m_converterStore));
   query.bindValue(properties.size(), mapping->getProperty(entity, mapping->getKeyProperty()));

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());
}

QSharedPointer<QObject> QormSession::selectOne(const QVariant &key, QormMetaEntity::Ptr mapping) {
   return selectOneBySql(m_entitySqlBuilder->buildSelect(mapping), mapping, QVariantList() << key);
}

QVariantList QormSession::selectOneBySql(const QString &sql, const QVariantList &parameters)
{
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw QormCouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());

   auto result = QVariantList();

   if (query.next())
   {
      auto record = query.record();
      for (auto i = 0; i < record.count(); i++)
         result.append(record.value(i));
   }

   if (query.next())
      throw QormMoreThanOneResultException();

   return result;
}

QSharedPointer<QObject> QormSession::selectOneBySql(const QString &sql, QormMetaEntity::Ptr mapping, const QVariantList &parameters) {
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw QormCouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());

   auto result = query.next() ?
      m_entityMapper->mapToEntity(mapping, query.record(), m_converterStore) :
      QSharedPointer<QObject>();

   if (query.next())
      throw QormMoreThanOneResultException();

   return result;
}

void QormSession::selectManyByCallback(
   QormMetaEntity::Ptr mapping,
   std::function<bool(const QSharedPointer<QObject> &)> callback,
   int skip,
   int pageSize) {
   QVariantList conditions;
   auto sql = m_entitySqlBuilder->buildSelectMany(mapping, skip, pageSize, conditions);
   selectManyByCallbackBySql(
      sql,
      mapping,
      callback,
      conditions);
}

void QormSession::selectManyByCallbackBySql(
   const QString &sql,
   QormMetaEntity::Ptr mapping,
   std::function<bool(const QSharedPointer<QObject> &)> callback,
   const QVariantList &parameters) {
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw QormCouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());

   bool continueWork = true;
   while (query.next() && continueWork)
      continueWork = callback(m_entityMapper->mapToEntity(mapping, query.record(), m_converterStore));
}

QList<QSharedPointer<QObject>> QormSession::selectMany(QormMetaEntity::Ptr mapping, int skip, int pageSize) {
   QList<QSharedPointer<QObject>> result;
   auto func = [&result](const QSharedPointer<QObject> &item) -> bool {
      result.append(item);
      return true;
   };
   selectManyByCallback(mapping, func, skip, pageSize);
   return result;
}

QList<QSharedPointer<QObject>> QormSession::selectManyBySql(const QString &sql, QormMetaEntity::Ptr mapping, const QVariantList &parameters) {
   QList<QSharedPointer<QObject>> result;
   auto func = [&result](const QSharedPointer<QObject> &item) -> bool {
      result.append(item);
      return true;
   };
   selectManyByCallbackBySql(sql, mapping, func, parameters);
   return result;
}

void QormSession::selectManyBySqlWithCustomMapping(
   const QString &sql,
   std::function<bool(const QOrmOnDemandRecordMapper *)> callback,
   const QVariantList &parameters) {
   
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw QormCouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw QormCouldNotExecuteQueryException(query.lastError());

   QOrmOnDemandRecordMapperImpl recordMapper([&](const QormMetaEntity::Ptr& mapping, const QString& prefix)
   {
      return m_entityMapper->mapToEntity(mapping, query.record(), m_converterStore);
   });

   bool continueWork = true;
   while (query.next() && continueWork)
      continueWork = callback(&recordMapper);
}
