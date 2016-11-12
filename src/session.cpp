#include <QMetaOrm/session.h>
#include <QMetaOrm/exceptions.h>
#include <qthread.h>

using namespace QMetaOrm;

QString GetThreadIdentifier() {
   return QString::number((int)QThread::currentThreadId());
}

Session::Session(
   DatabaseFactory::Ptr databaseFactory,
   EntitySqlBuilder::Ptr entitySqlBuilder,
   EntityMapper::Ptr entityMapper,
   ConverterStore::Ptr converterStore)
   : m_database(databaseFactory->createDatabase(GetThreadIdentifier()))
   , m_entitySqlBuilder(entitySqlBuilder)
   , m_entityMapper(entityMapper)
   , m_converterStore(converterStore)
{
}

Session::~Session() {
}

void Session::commit() {
   if (!m_database.commit())
      throw TransactionException(m_database.lastError());
}

void Session::rollback() {
   if (!m_database.rollback())
      throw TransactionException(m_database.lastError());
}

void Session::setupSession() {
   if (m_database.isOpen())
      return;

   if (!m_database.open())
      throw ConnectToDatabaseException(m_database.lastError());

   if (!m_database.transaction())
      throw TransactionException(m_database.lastError());
}
