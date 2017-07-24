#include <QMetaOrm/QormMetaEntityBuilder.h>
#include <QMetaOrm/QormSessionFactoryBuilder.h>
#include <QMetaOrm/QormExceptions.h>

#include <QtTest>
#include <QtSql>

#include "DatabaseFactory.h"
#include "TestMacros.h"
#include "SqlHelper.h"
#include "Person.h"

class AnyBuilder {
public:
    static QString anyString(int size = 10)
    {
        return QString().fill('A', size);
    }
};

class ToUpperConverter : public QormConverter {
public:
    QVariant convert(const QVariant& value) const override
    {
        return value.toString().toUpper();
    }
};

/*
   TODO:
   - Caching when save() and select() is called in sequence.
*/
class QormDefaultSessionIT : public QObject {
Q_OBJECT

private:
    QormSessionFactory::Ptr m_sessionFactory;
    SqlHelper::Ptr m_sqlHelper;

public:
    QormDefaultSessionIT() { }

private Q_SLOTS :

    void init()
    {
        auto databaseFactory = SQLiteEmbeddedDatabaseFactory::factory();
        m_sessionFactory = QormSessionFactoryBuilder::AFactory()
            .withDatabase(databaseFactory)
            .build();
        m_sqlHelper = SqlHelper::factory(databaseFactory);
    }

    void cleanup()
    {
        m_sqlHelper.clear();
        m_sessionFactory.clear();
    }

    void save_newPerson_personIsPersistetAndIdIsSet()
    {
        auto person = PersonSimple::Ptr(new PersonSimple());
        person->setName("Mueller");
        person->setSurname("Hans");

        auto session = m_sessionFactory->createSession();
        session->save(person, QormMappings::TsPersonSimpleMapping());
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 1);
        QCOMPARE(records[0][0].toInt(), person->getId());
        QCOMPARE(records[0][1].toString(), person->getName());
        QCOMPARE(records[0][2].toString(), person->getSurname());
    }

    void save_newPersonWithCustomMapping_onlyFieldsOfThatMappingArePersistet()
    {
        auto metaEntity = QormMetaEntityBuilder::anEntity()
            .forSource("PERSON")
            .withId(PersonSimple::p::id, "ID")
            .withData(PersonSimple::p::name, "NAME")
            .build<PersonSimple>();

        auto person = PersonSimple::Ptr(new PersonSimple());
        person->setName("Mueller");
        person->setSurname("Hans");

        auto session = m_sessionFactory->createSession();
        session->save(person, metaEntity);
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 1);
        QCOMPARE(records[0][0].toInt(), person->getId());
        QCOMPARE(records[0][1].toString(), person->getName());
        QCOMPARE(records[0][2].toString(), QString());
    }

    void save_newPersonWithAddressRelation_personAndReferencedAddressIdIsPersistetButAddressDatasetStaysUntouched()
    {
        int idAddress = 1;
        QString country = AnyBuilder::anyString(), postCode = AnyBuilder::anyString(), street = AnyBuilder::anyString();
        m_sqlHelper->insert("insert into address (id, country, postCode, street) values (?,?,?,?)",
            QVariantList() << idAddress << country << postCode << street);

        auto address = Address::Ptr(new Address());
        address->setId(idAddress);

        auto person = PersonComplex::Ptr(new PersonComplex());
        person->setName(AnyBuilder::anyString());
        person->setAddress(address);

        auto session = m_sessionFactory->createSession();
        session->save(person, QormMappings::TsPersonComplexMapping());
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
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << idPerson << name << "Otto");

        auto person = PersonSimple::Ptr(new PersonSimple());
        person->setId(idPerson);
        person->setName(name);
        person->setSurname(surname);

        auto session = m_sessionFactory->createSession();
        session->save(person, QormMappings::TsPersonSimpleMapping());
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 1);
        QCOMPARE(records[0][0].toInt(), person->getId());
        QCOMPARE(records[0][1].toString(), person->getName());
        QCOMPARE(records[0][2].toString(), person->getSurname());
    }

    void save_sqlToInsertOnePersonWithTwoParameters_personGetsInserted()
    {
        QString name = "Hans", surname = "Otto";

        auto session = m_sessionFactory->createSession();
        session->save(QormSql("insert into person (id, name, surname) values (1, ?, ?)", QVariantList() << name << surname));
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 1);
        QCOMPARE(records[0][0].toInt(), 1);
        QCOMPARE(records[0][1].toString(), name);
        QCOMPARE(records[0][2].toString(), surname);
    }

    void remove_existingPerson_personIsDeleted()
    {
        int idPerson = 1;
        QString name = "Mueller", surname = "Hans";
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << idPerson << name << surname);

        auto person = PersonSimple::Ptr(new PersonSimple());
        person->setId(idPerson);
        person->setName(name);
        person->setSurname(surname);

        auto session = m_sessionFactory->createSession();
        session->remove(person, QormMappings::TsPersonSimpleMapping());
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 0);
    }

    void selectOne_sqlThatReturnsNoResult_nullptr()
    {
        auto session = m_sessionFactory->createSession();

        auto item = session->selectOne<PersonSimple>(QormSql("select * from person limit 1"),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(item==nullptr);
    }

    void selectOne_sqlThatReturnsOneResult_thatPerson()
    {
        QString name = "Mueller", surname = "Hans";
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << name << surname);

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql("select * from person limit 1"),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(item!=nullptr);
        QCOMPARE(item->getId(), id);
        QCOMPARE(item->getName(), name);
        QCOMPARE(item->getSurname(), surname);
    }

    void selectOne_sqlThatReturnsTwoResults_exception()
    {
        QString sql = "insert into person (id, name, surname) values (?,?,?)";
        m_sqlHelper->insert(sql, QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
        m_sqlHelper->insert(sql, QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        VERIFY_EXCEPTION_THROWN(
            session->selectOne<PersonSimple>(QormSql("select * from person"), QormMappings::TsPersonSimpleMapping()),
            QormMoreThanOneResultException);
    }

    void selectOne_sqlThatSelectsOnlyName_onePersonWithNameOnlyIsFilled()
    {
        QString name = "Mueller";
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << name << "Hans");

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql("select name from person limit 1"),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(item!=nullptr);
        QCOMPARE(item->getId(), 0);
        QCOMPARE(item->getSurname(), QString());
        QCOMPARE(item->getName(), name);
    }

    void selectOne_sqlThatReturnsBothTimesTheSameResult_bothResultsShouldBeIdenticalBecauseOfCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql("select id from person limit 1"),
            QormMappings::TsPersonSimpleMapping());
        auto item2 = session->selectOne<PersonSimple>(QormSql("select id from person limit 1"),
            QormMappings::TsPersonSimpleMapping());

        QCOMPARE(item, item2);
    }

    void selectOne_sqlThatReturnsBothTimesTheSameResultWithName_bothResultsShouldDifferentBecauseOfNoCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql("select name from person limit 1"),
            QormMappings::TsPersonSimpleMapping());
        auto item2 = session->selectOne<PersonSimple>(QormSql("select name from person limit 1"),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(item!=item2);
    }

    void selectOne_sqlThatReturnsAnResultByAConcreteId_thePersonWithThatId()
    {
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(
            QormSql("select * from person where id = ? limit 1", QVariantList() << id),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(item!=nullptr);
        QCOMPARE(item->getId(), id);
    }

    void selectOne_noPersonExists_nullptr()
    {
        auto session = m_sessionFactory->createSession();

        auto item = session->selectOne<PersonSimple>(1, QormMappings::TsPersonSimpleMapping());

        QVERIFY(item==nullptr);
    }

    void selectOne_onePersonIsSelected_thatPerson()
    {
        QString name = "Mueller", surname = "Hans";
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << name << surname);

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(id, QormMappings::TsPersonSimpleMapping());

        QVERIFY(item!=nullptr);
        QCOMPARE(item->getId(), id);
        QCOMPARE(item->getName(), name);
        QCOMPARE(item->getSurname(), surname);
    }

    void selectOne_onePersonWithIdIsSelectedTwice_bothResultsShouldBeIdenticalBecauseOfCaching()
    {
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(id, QormMappings::TsPersonSimpleMapping());
        auto item2 = session->selectOne<PersonSimple>(id, QormMappings::TsPersonSimpleMapping());

        QCOMPARE(item, item2);
    }

    void selectOne_personWithOneToManyRelation_relationWillBeFilled()
    {
        QString country = "Germany", postCode = "55555", street = "Mueller Street 1";
        int idPerson = 1, idAddress = 1;
        m_sqlHelper->insert("insert into address (id, country, postCode, street) values (?,?,?,?)",
            QVariantList() << idAddress << country << postCode << street);
        m_sqlHelper->insert("insert into person (id, name, surname, address) values (?,?,?,?)",
            QVariantList() << idPerson << AnyBuilder::anyString() << AnyBuilder::anyString() << idAddress);

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonComplex>(idPerson, QormMappings::TsPersonComplexMapping());

        QVERIFY(item!=nullptr && item->getAddress()!=nullptr);
        QCOMPARE(item->getAddress()->getId(), idAddress);
        QCOMPARE(item->getAddress()->getCountry(), country);
        QCOMPARE(item->getAddress()->getPostCode(), postCode);
        QCOMPARE(item->getAddress()->getStreet(), street);
    }

    void selectOne_mappingWithConverter_valueGetsConverted()
    {
        auto mapping = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withId("id", "ID")
            .withConvertedData<ToUpperConverter>("upperName", "NAME")
            .build<PersonSimple>();

        int idPerson = 1;
        QString name = "Mueller", surname = "Hans";
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << idPerson << name << surname);

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(1, mapping);

        QVERIFY(item!=nullptr);
        QCOMPARE(QString("MUELLER"), item->property("upperName").toString());
    }

    void selectManyBySql_noPersons_emptyList()
    {
        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormSql("select * from person"),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(items.isEmpty());
    }

    void selectManyBySql_select2Persons_listOf2Persons()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormSql("select * from person"),
            QormMappings::TsPersonSimpleMapping());

        QCOMPARE(items.size(), 2);
    }

    void selectManyBySql_selectOnePersonWithIdTwoTimes_bothVersionsAreIdenticalDueToCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormSql("select id from person limit 1"),
            QormMappings::TsPersonSimpleMapping());
        auto items2 = session->selectMany<PersonSimple>(QormSql("select id from person limit 1"),
            QormMappings::TsPersonSimpleMapping());

        QCOMPARE(items[0], items2[0]);
    }

    void selectManyBySql_onePersonIsSelectedById_thePersonWithThatId()
    {
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(
            QormSql("select * from person where id = ? limit 1", QVariantList() << id),
            QormMappings::TsPersonSimpleMapping());

        QVERIFY(items.size()==1);
        QCOMPARE(items[0]->getId(), id);
    }

    void selectMany_noPersons_emptyList()
    {
        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormMappings::TsPersonSimpleMapping());

        QVERIFY(items.isEmpty());
    }

    void selectMany_select2Persons_listOf2Persons()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormMappings::TsPersonSimpleMapping());

        QCOMPARE(items.size(), 2);
    }

    void selectMany_selectOnePersonWithIdTwoTimes_bothVersionsAreIdenticalDueToCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormMappings::TsPersonSimpleMapping());
        auto items2 = session->selectMany<PersonSimple>(QormMappings::TsPersonSimpleMapping());

        QCOMPARE(items[0], items2[0]);
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionIT)

#include "QormDefaultSessionIT.moc"
