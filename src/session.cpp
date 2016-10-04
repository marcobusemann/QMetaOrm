#include <QMetaOrm/session.h>
#include <QMetaOrm/exceptions.h>
#include <qthread.h>

using namespace QMetaOrm;

QString GetThreadIdentifier() {
   return QString::number((int)QThread::currentThreadId());
}

//-----------------------------------------------------------------------------
Session::Session(
   DatabaseFactory::Ptr databaseFactory,
   EntitySqlBuilder::Ptr entitySqlBuilder,
   EntityMapper::Ptr entityMapper)
   : m_database(databaseFactory->createDatabase(GetThreadIdentifier()))
   , m_entitySqlBuilder(entitySqlBuilder)
   , m_entityMapper(entityMapper)
{
}

//-----------------------------------------------------------------------------
Session::~Session() {
   m_database.rollback();
   m_database.close();
}

//-----------------------------------------------------------------------------
void Session::commit() {
   if (!m_database.commit())
      throw TransactionException(m_database.lastError());
}

//-----------------------------------------------------------------------------
void Session::rollback() {
   if (!m_database.rollback())
      throw TransactionException(m_database.lastError());
}

//-----------------------------------------------------------------------------
void Session::setupSession() {
   if (m_database.isOpen())
      return;

   if (!m_database.open())
      throw ConnectToDatabaseException(m_database.lastError());

   if (!m_database.transaction())
      throw TransactionException(m_database.lastError());
}
