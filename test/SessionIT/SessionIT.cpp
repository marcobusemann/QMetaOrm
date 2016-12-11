#include "DatabaseFactory.h"
#include "Person.h"

#include <QtTest>

#include <QMetaOrm/MetaEntityBuilder.h>
#include <QMetaOrm/ValueCriterion.h>
#include <QMetaOrm/SessionFactory.h>
#include <QMetaOrm/Session.h>

using namespace QMetaOrm;

class SqlHelper
{
public:
   typedef QSharedPointer<SqlHelper> Ptr;
   static Ptr factory(DatabaseFactory::Ptr databaseFactory) {
      return Ptr(new SqlHelper(databaseFactory));
   }

public:
   SqlHelper(DatabaseFactory::Ptr databaseFactory):
      m_databaseFactory(databaseFactory)
   {
   }

   void insert(const QString &aSql, const QVariantList &params = QVariantList()) {
      auto database = m_databaseFactory->createDatabase();
      QSqlQuery query(database);
      bool okPrepare = query.prepare(aSql);
      Q_ASSERT(okPrepare);

      for (int i = 0; i < params.size(); i++)
         query.bindValue(i, params[i]);
      bool okExec = query.exec();
      Q_ASSERT(okExec);
   }

   QList<QVariantList> select(const QString &aSql, const QVariantList &params = QVariantList()) {
      auto database = m_databaseFactory->createDatabase();
      QSqlQuery query(database);
      bool okPrepare = query.prepare(aSql);
      Q_ASSERT(okPrepare);

      for (int i = 0; i < params.size(); i++)
         query.bindValue(i, params[i]);
      bool okExec = query.exec();
      Q_ASSERT(okExec);

      QList<QVariantList> result;
      while (query.next()) {
         QVariantList record;
         for (int i = 0; i < query.record().count(); i++)
            record << query.value(i);
         result << record;
      }
      return result;
   }

private:
   DatabaseFactory::Ptr m_databaseFactory;
};

class AnyBuilder {
public:
   static QString anyString(int size = 10) {
      return QString().fill('A', size);
   }
};

/*
   TODO:
   - Caching when save() and select() is called in sequence.
*/
class SessionIT : public QObject
{
   Q_OBJECT

private:
   DatabaseFactory::Ptr m_databaseFactory;
   SessionFactory::Ptr m_sessionFactory;
   SqlHelper::Ptr m_sqlHelper;

public:
   SessionIT() {}

private Q_SLOTS :

   void init()
   {
      m_databaseFactory = TestDatabaseFactory::factory();
      m_sessionFactory = DefaultSessionFactory::factory(
         m_databaseFactory,
         StandardQtLogger::factory(),
         StandardEntityCacheFactory::factory());
      m_sqlHelper = SqlHelper::factory(m_databaseFactory);
   }

   void cleanup() {
      m_sqlHelper.clear();
      m_sessionFactory.clear();
      m_databaseFactory.clear();
   }

   void selectOneBySql_noPersonExists_nullptr()
   {
      auto session = m_sessionFactory->createSession();

      auto item = session->selectOneBySql<PersonSimple>("select first 1 * from person");

      QVERIFY(item == nullptr);
   }

   void selectOneBySql_onePersonIsSelected_thatPerson()
   {
      QString name = "Müller", surname = "Hans";
      int id = 1;
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << id << name << surname);

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOneBySql<PersonSimple>("select first 1 * from person");

      QVERIFY(item != nullptr);
      QCOMPARE(item->getId(), id);
      QCOMPARE(item->getName(), name);
      QCOMPARE(item->getSurname(), surname);
   }

   void selectOneBySql_twoPersonsAreSelected_exception()
   {
      QString sql = "insert into person (id, name, surname) values (?,?,?)";
      m_sqlHelper->insert(sql, QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
      m_sqlHelper->insert(sql, QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      QVERIFY_EXCEPTION_THROWN(session->selectOneBySql<PersonSimple>("select * from person"), MoreThanOneResultException);
   }

   void selectOneBySql_onePersonWithNameOnlyIsSelected_onePersonWithNameOnlyIsFilled()
   {
      QString name = "Müller";
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << name << "Hans");

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOneBySql<PersonSimple>("select first 1 name from person");

      QVERIFY(item != nullptr);
      QCOMPARE(item->getId(), 0);
      QCOMPARE(item->getSurname(), QString());
      QCOMPARE(item->getName(), name);
   }

   void selectOneBySql_onePersonWithIdIsSelectedTwice_bothResultsShouldBeIdenticalBecauseOfCaching()
   {
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOneBySql<PersonSimple>("select first 1 id from person");
      auto item2 = session->selectOneBySql<PersonSimple>("select first 1 id from person");

      QCOMPARE(item, item2);
   }

   void selectOneBySql_onePersonWithNameIsSelectedTwice_bothResultsShouldDifferentBecauseOfNoCaching()
   {
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOneBySql<PersonSimple>("select first 1 name from person");
      auto item2 = session->selectOneBySql<PersonSimple>("select first 1 name from person");

      QVERIFY(item != item2);
   }

   void selectOneBySql_onePersonIsSelectedById_thePersonWithThatId()
   {
      int id = 1;
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOneBySql<PersonSimple>("select first 1 * from person where id = ?", QVariantList() << id);

      QVERIFY(item != nullptr);
      QCOMPARE(item->getId(), id);
   }

   void selectOne_noPersonExists_nullptr()
   {
      auto session = m_sessionFactory->createSession();

      auto item = session->selectOne<PersonSimple>(1);

      QVERIFY(item == nullptr);
   }

   void selectOne_onePersonIsSelected_thatPerson()
   {
      QString name = "Müller", surname = "Hans";
      int id = 1;
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << id << name << surname);

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOne<PersonSimple>(id);

      QVERIFY(item != nullptr);
      QCOMPARE(item->getId(), id);
      QCOMPARE(item->getName(), name);
      QCOMPARE(item->getSurname(), surname);
   }

   void selectOne_onePersonWithIdIsSelectedTwice_bothResultsShouldBeIdenticalBecauseOfCaching()
   {
      int id = 1;
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOne<PersonSimple>(id);
      auto item2 = session->selectOne<PersonSimple>(id);

      QCOMPARE(item, item2);
   }

   void selectOne_personWithOneToManyRelation_relationWillBeFilled()
   {
      QString country = "Germany", postCode = "55555", street = "Müller Street 1";
      int idPerson = 1, idAddress = 1;
      m_sqlHelper->insert("insert into address (id, country, postCode, street) values (?,?,?,?)", QVariantList() << idAddress << country << postCode << street);
      m_sqlHelper->insert("insert into person (id, name, surname, address) values (?,?,?,?)", QVariantList() << idPerson << AnyBuilder::anyString() << AnyBuilder::anyString() << idAddress);

      auto session = m_sessionFactory->createSession();
      auto item = session->selectOne<PersonComplex>(idPerson);

      QVERIFY(item != nullptr && item->getAddress() != nullptr);
      QCOMPARE(item->getAddress()->getId(), idAddress);
      QCOMPARE(item->getAddress()->getCountry(), country);
      QCOMPARE(item->getAddress()->getPostCode(), postCode);
      QCOMPARE(item->getAddress()->getStreet(), street);
   }

   void selectManyBySql_noPersons_emptyList() 
   {
      auto session = m_sessionFactory->createSession();
      auto items = session->selectManyBySql<PersonSimple>("select * from person");

      QVERIFY(items.isEmpty());
   }

   void selectManyBySql_select2Persons_listOf2Persons()
   {
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto items = session->selectManyBySql<PersonSimple>("select * from person");

      QCOMPARE(items.size(), 2);
   }

   void selectManyBySql_selectOnePersonWithIdTwoTimes_bothVersionsAreIdenticalDueToCaching()
   {
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto items  = session->selectManyBySql<PersonSimple>("select first 1 id from person");
      auto items2 = session->selectManyBySql<PersonSimple>("select first 1 id from person");

      QCOMPARE(items[0], items2[0]);
   }

   void selectManyBySql_onePersonIsSelectedById_thePersonWithThatId()
   {
      int id = 1;
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto items = session->selectManyBySql<PersonSimple>("select first 1 * from person where id = ?", QVariantList() << id);

      QVERIFY(items.size() == 1);
      QCOMPARE(items[0]->getId(), id);
   }

   void selectMany_noPersons_emptyList()
   {
      auto session = m_sessionFactory->createSession();
      auto items = session->selectMany<PersonSimple>();

      QVERIFY(items.isEmpty());
   }

   void selectMany_select2Persons_listOf2Persons()
   {
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto items = session->selectMany<PersonSimple>();

      QCOMPARE(items.size(), 2);
   }

   void selectMany_selectOnePersonWithIdTwoTimes_bothVersionsAreIdenticalDueToCaching()
   {
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto items = session->selectMany<PersonSimple>();
      auto items2 = session->selectMany<PersonSimple>();

      QCOMPARE(items[0], items2[0]);
   }

   void selectMany_onePersonIsSelectedById_thePersonWithThatId()
   {
      int id = 1;
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 3 << AnyBuilder::anyString() << AnyBuilder::anyString());
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

      auto session = m_sessionFactory->createSession();
      auto items = session->selectMany<PersonSimple>(ValueCriterion::equals(PersonSimple::p::id, id));

      QVERIFY(items.size() == 1);
      QCOMPARE(items[0]->getId(), id);
   }
   
   void save_newPerson_personIsPersistetAndIdIsSet() 
   {
      auto person = PersonSimple::Ptr(new PersonSimple());
      person->setName("Müller");
      person->setSurname("Hans");

      auto session = m_sessionFactory->createSession();
      session->save(person);
      session->commit();

      auto records = m_sqlHelper->select("select id, name, surname from person");
      QCOMPARE(records.size(), 1);
      QCOMPARE(records[0][0].toInt(), person->getId());
      QCOMPARE(records[0][1].toString(), person->getName());
      QCOMPARE(records[0][2].toString(), person->getSurname());
   }

   void save_newPersonWithCustomMapping_onlyFieldsOfThatMappingArePersistet()
   {
      auto metaEntity = MetaEntityBuilder::anEntity()
         .forSource("PERSON")
         .withSequence("PERSON_GEN")
         .withId(PersonSimple::p::id, "ID")
         .withData(PersonSimple::p::name, "NAME")
         .build<PersonSimple>();

      auto person = PersonSimple::Ptr(new PersonSimple());
      person->setName("Müller");
      person->setSurname("Hans");

      auto session = m_sessionFactory->createSession();
      session->save(person.objectCast<QObject>(), metaEntity);
      session->commit();

      auto records = m_sqlHelper->select("select id, name, surname from person");
      QCOMPARE(records.size(), 1);
      QCOMPARE(records[0][0].toInt(), person->getId());
      QCOMPARE(records[0][1].toString(), person->getName());
      QCOMPARE(records[0][2], QVariant());
   }

   void save_newPersonWithAddressRelation_personAndReferencedAddressIdIsPersistetButAddressDatasetStaysUntuched()
   {
      int idAddress = 1;
      QString country = AnyBuilder::anyString(), postCode = AnyBuilder::anyString(), street = AnyBuilder::anyString();
      m_sqlHelper->insert("insert into address (id, country, postCode, street) values (?,?,?,?)", QVariantList() << idAddress << country << postCode << street);

      auto address = Address::Ptr(new Address());
      address->setId(idAddress);

      auto person = PersonComplex::Ptr(new PersonComplex());
      person->setAddress(address);

      auto session = m_sessionFactory->createSession();
      session->save(person);
      session->commit();

      auto personRecords = m_sqlHelper->select("select id, address from person");
      QCOMPARE(personRecords.size(), 1);
      QCOMPARE(personRecords[0][0].toInt(), person->getId());
      QCOMPARE(personRecords[0][1].toInt(), address->getId());
 
      auto addressRecords = m_sqlHelper->select("select id, country, postCode, street from address");
      QCOMPARE(addressRecords.size(), 1);
      QCOMPARE(addressRecords[0][0].toInt(), address->getId());
      QCOMPARE(addressRecords[0][1].toString(), country);
      QCOMPARE(addressRecords[0][1].toString(), postCode);
      QCOMPARE(addressRecords[0][1].toString(), street);
   }

   void save_existingPerson_personIsUpdated()
   {
      int idPerson = 1;
      QString name = AnyBuilder::anyString(), surname = "Hans";
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << idPerson << name << "Otto");

      auto person = PersonSimple::Ptr(new PersonSimple());
      person->setId(idPerson);
      person->setName(name);
      person->setSurname(surname);

      auto session = m_sessionFactory->createSession();
      session->save(person);
      session->commit();

      auto records = m_sqlHelper->select("select id, name, surname from person");
      QCOMPARE(records.size(), 1);
      QCOMPARE(records[0][0].toInt(), person->getId());
      QCOMPARE(records[0][1].toString(), person->getName());
      QCOMPARE(records[0][2].toString(), person->getSurname());
   }

   void remove_existingPerson_personIsDeleted()
   {
      int idPerson = 1;
      QString name = "Müller", surname = "Hans";
      m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)", QVariantList() << idPerson << name << surname);

      auto person = PersonSimple::Ptr(new PersonSimple());
      person->setId(idPerson);
      person->setName(name);
      person->setSurname(surname);

      auto session = m_sessionFactory->createSession();
      session->remove(person);
      session->commit();

      auto records = m_sqlHelper->select("select id, name, surname from person");
      QCOMPARE(records.size(), 0);
   }
};

QTEST_APPLESS_MAIN(SessionIT)

#include "SessionIT.moc"
