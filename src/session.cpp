#include <QMetaOrm/Session.h>
#include <QMetaOrm/Exceptions.h>
#include <quuid.h>

using namespace QMetaOrm;

QString GetThreadIdentifier() {
   return QUuid::createUuid().toString();
}

Session::Session(
   DatabaseFactory::Ptr databaseFactory,
   EntitySqlBuilder::Ptr entitySqlBuilder,
   EntityMapper::Ptr entityMapper,
   ConverterStore::Ptr converterStore)
   : m_database(databaseFactory->createDatabase(GetThreadIdentifier()))
   , m_entityMapper(entityMapper)
   , m_entitySqlBuilder(entitySqlBuilder)
   , m_converterStore(converterStore)
{
}

Session::~Session() {
   rollback();
}

void Session::commit() {
   m_database.commit();
}

void Session::rollback() {
   m_database.rollback();
}

void Session::setupSession() {
   if (!m_database.isOpen() && !m_database.open())
      throw ConnectToDatabaseException(m_database.lastError());

   m_database.transaction();
}

void Session::save(QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping)
{
   setupSession();
   if (mapping->hasValidKey(entity))
      return update(entity, mapping);
   else
      return create(entity, mapping);
}

void Session::remove(const QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping)
{
   setupSession();
   Q_ASSERT_X(mapping->hasValidKey(entity), "remove", "entity has no valid key, removing not possible.");

   QSqlQuery query(m_database);

   if (!query.prepare(m_entitySqlBuilder->buildRemove(mapping)))
      throw CouldNotPrepareQueryException(query.lastError());

   query.bindValue(0, mapping->getProperty(entity, mapping->getKeyProperty()));

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());
}

void Session::create(QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping) {
   setupSession();

   QSqlQuery query(m_database);

   QStringList properties;
   if (!query.prepare(m_entitySqlBuilder->buildInsert(mapping, properties)))
      throw CouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < properties.size(); i++)
      query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i], m_converterStore));

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   if (query.first())
      mapping->setProperty(entity, mapping->getKeyProperty(), query.value(0));
}

void Session::update(QSharedPointer<QObject> &entity, MetaEntity::Ptr mapping) {
   setupSession();

   QSqlQuery query(m_database);

   QStringList properties;

   if (!query.prepare(m_entitySqlBuilder->buildUpdate(mapping, properties)))
      throw CouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < properties.size(); i++)
      query.bindValue(i, mapping->getFlatPropertyValue(entity, properties[i], m_converterStore));
   query.bindValue(properties.size(), mapping->getProperty(entity, mapping->getKeyProperty()));

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());
}

QSharedPointer<QObject> Session::selectOne(const QVariant &key, MetaEntity::Ptr mapping) {
   return selectOneBySql(m_entitySqlBuilder->buildSelect(mapping), mapping, QVariantList() << key);
}

QSharedPointer<QObject> Session::selectOneBySql(const QString &sql, MetaEntity::Ptr mapping, const QVariantList &parameters) {
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw CouldNotPrepareQueryException(query.lastError());

   for(int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   auto result = query.next() ?
      m_entityMapper->mapToEntity(mapping, query.record(), m_converterStore) :
      QSharedPointer<QObject>();

   if (query.next())
      throw MoreThanOneResultException();

   return result;
}

void Session::selectManyByCallback(
   MetaEntity::Ptr mapping,
   std::function<bool(const QSharedPointer<QObject> &)> callback,
   Criterion::Ptr criterion,
   int skip,
   int pageSize) {
   QVariantList conditions;
   auto sql = m_entitySqlBuilder->buildSelectMany(mapping, criterion, skip, pageSize, conditions);
   selectManyByCallbackBySql(
      sql,
      mapping,
      callback,
      conditions);
}

void Session::selectManyByCallbackBySql(
   const QString &sql,
   MetaEntity::Ptr mapping,
   std::function<bool(const QSharedPointer<QObject> &)> callback,
   const QVariantList &parameters) {
   setupSession();

   QSqlQuery query(m_database);

   if (!query.prepare(sql))
      throw CouldNotPrepareQueryException(query.lastError());

   for (int i = 0; i < parameters.size(); i++)
      query.bindValue(i, parameters[i]);

   if (!query.exec())
      throw CouldNotExecuteQueryException(query.lastError());

   bool continueWork = true;
   while (query.next() && continueWork)
      continueWork = callback(m_entityMapper->mapToEntity(mapping, query.record(), m_converterStore));
}

QList<QSharedPointer<QObject>> Session::selectMany(MetaEntity::Ptr mapping, Criterion::Ptr criterion, int skip, int pageSize) {
   QList<QSharedPointer<QObject>> result;
   auto func = [&result](const QSharedPointer<QObject> &item) -> bool {
      result.append(item);
      return true;
   };
   selectManyByCallback(mapping, func, criterion, skip, pageSize);
   return result;
}

QList<QSharedPointer<QObject>> Session::selectManyBySql(const QString &sql, MetaEntity::Ptr mapping, const QVariantList &parameters) {
   QList<QSharedPointer<QObject>> result;
   auto func = [&result](const QSharedPointer<QObject> &item) -> bool {
      result.append(item);
      return true;
   };
   selectManyByCallbackBySql(sql, mapping, func, parameters);
   return result;
}
