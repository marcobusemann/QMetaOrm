#include <QMetaOrm\DatabaseFactory.h>

#include <qstringlist.h>
#include <quuid.h>
#include <qfile.h>

using namespace QMetaOrm;

class TestDatabaseFactory : public DatabaseFactory
{
public:
   static Ptr factory() {
      return Ptr(new TestDatabaseFactory());
   }

public:
   TestDatabaseFactory() {
      m_file = QUuid::createUuid().toString().remove("{").remove("}").remove("-") + ".fdb";
      QFile::copy(":/database.fdb", m_file);

      QFile file(m_file);
      file.open(QFile::WriteOnly);
      file.setPermissions(QFileDevice::ExeOther | QFileDevice::ReadOther | QFileDevice::WriteOther);
      file.close();
   }

   ~TestDatabaseFactory() {
      cleanup();
   }

   virtual QSqlDatabase createDatabase(const QString & name = QString()) const override
   {
      if (!QSqlDatabase::contains(name)) {
         QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE", name);
         db.setDatabaseName(m_file);
         db.setUserName("SYSDBA");
         db.setPassword("MASTERKEY");
      }
      return QSqlDatabase::database(name, true);
   }

   void cleanup() {
      for (auto name : QSqlDatabase::connectionNames())
         QSqlDatabase::removeDatabase(name);
      QFile::remove(m_file);
   }

private:
   QString m_file;
};
