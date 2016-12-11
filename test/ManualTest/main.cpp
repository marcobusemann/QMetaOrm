#include <QCoreApplication>
#include "main.h"
#include <QMetaOrm/metaentitybuilder.h>
#include <QMetaOrm/databasefactory.h>

const QString PersonSimple::p::id = "id";
const QString PersonSimple::p::surname = "surname";
const QString PersonSimple::p::lastname = "lastname";
const QString PersonSimple::p::birthdate = "birthdate";
const QString PersonSimple::p::address = "address";

const QString Address::p::id = "id";
const QString Address::p::country = "country";
const QString Address::p::street = "street";
const QString Address::p::plz = "plz";

namespace QMetaOrm {
   namespace Mappings {
      template <> QMetaOrm::MetaEntity::Ptr mapping<Address>() {
         static const MetaEntity::Ptr map = MetaEntityBuilder::anEntity()
            .forSource("address")
            .withSequence("address_gen")
            .withId(Address::p::id, "ID")
            .withData(Address::p::country, "COUNTRY")
            .withData(Address::p::street, "STREET")
            .withData(Address::p::plz, "PLZ")
            .withEmbeddedPtrNamingScheme()
            .build<Address>();
         return map;
      }
   }
}

namespace QMetaOrm {
   namespace Mappings {
      template <> QMetaOrm::MetaEntity::Ptr mapping<PersonSimple>() {
         static const MetaEntity::Ptr map = MetaEntityBuilder::anEntity()
            .forSource("person")
            .withSequence("person_gen")
            .withId(PersonSimple::p::id, "ID")
            .withData(PersonSimple::p::surname, "SURNAME")
            .withData(PersonSimple::p::lastname, "NAME")
            .withData(PersonSimple::p::birthdate, "BIRTHDATE")
            .withOneToMany(PersonSimple::p::address, "id_address", mapping<Address>())
            .withEmbeddedPtrNamingScheme()
            .build<PersonSimple>();
         return map;
      }
   }
}

class MyDatabaseFactory : public QMetaOrm::DatabaseFactory {
public:
   static QMetaOrm::DatabaseFactory::Ptr create() {
      return QMetaOrm::DatabaseFactory::Ptr(new MyDatabaseFactory());
   }

   virtual QSqlDatabase createDatabase(const QString &name = QString()) const {
      if (!QSqlDatabase::contains(name)) {
         QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE", name);
         db.setHostName("localhost");
         db.setDatabaseName("D:\\Development\\OrmExample\\Database\\test.fdb");
         db.setUserName("sysdba");
         db.setPassword("masterkey");
         return db;
      }
      else
         return QSqlDatabase::database(name);
   }
};

#include <QMetaOrm/sessionfactory.h>

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   qDebug() << QSqlDatabase::drivers();
   qDebug() << QCoreApplication::libraryPaths();

   try {
      auto databaseFactory = MyDatabaseFactory::create();
      auto sessionFactory = QMetaOrm::DefaultSessionFactory::factory(databaseFactory);

      auto session = sessionFactory->createSession();
      auto items = session->selectMany<PersonSimple>();

      for (auto item : items)
         item->dump();
   }
   catch (std::exception e) {
      qDebug() << e.what();
   }

   return a.exec();
}
