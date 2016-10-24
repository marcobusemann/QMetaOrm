#include <QCoreApplication>
#include "main.h"
#include <QMetaOrm/metaentitybuilder.h>
#include <QMetaOrm/databasefactory.h>

const QString Person::p::id = "id";
const QString Person::p::surname = "surname";
const QString Person::p::lastname = "lastname";
const QString Person::p::birthdate = "birthdate";
const QString Person::p::address = "address";

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
		template <> QMetaOrm::MetaEntity::Ptr mapping<Person>() {
			static const MetaEntity::Ptr map = MetaEntityBuilder::anEntity()
				.forSource("person")
				.withSequence("person_gen")
				.withId(Person::p::id, "ID")
				.withData(Person::p::surname, "SURNAME")
				.withData(Person::p::lastname, "NAME")
				.withData(Person::p::birthdate, "BIRTHDATE")
				.withReference(Person::p::address, "id_address", mapping<Address>())
				.withEmbeddedPtrNamingScheme()
				.build<Person>();
			return map;
		}
	}
}

class MyDatabaseFactory: public QMetaOrm::DatabaseFactory {
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
	   auto items = session->selectMany<Person>();

	   for (auto item : items)
		   item->dump();
    }
    catch (std::exception e) {
       qDebug() << e.what();
    }

    return a.exec();
}
