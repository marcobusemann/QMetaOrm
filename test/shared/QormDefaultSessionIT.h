#include <QMetaOrm/QormMetaEntityBuilder.h>
#include <QMetaOrm/QormSessionFactoryBuilder.h>
#include <QMetaOrm/QormExceptions.h>

#include <QtTest>
#include <QtSql>
#include <QSharedPointer>

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

class Mappings
{
public:
    typedef QSharedPointer<Mappings> Ptr;

    virtual ~Mappings() {}
    virtual QormMetaEntityBuilder personMappingWithSetupSourceAndSequence() = 0;
    virtual QormMetaEntity::Ptr personSimpleMapping() = 0;
    virtual QormMetaEntity::Ptr personComplexMapping() = 0;
    virtual QormMetaEntity::Ptr addressMapping() = 0;
};

class SqlQueries
{
public:
    typedef QSharedPointer<SqlQueries> Ptr;

    virtual ~SqlQueries() { }
    
    virtual QString selectOne() = 0;
    virtual QString selectOne(const QStringList &aColumnNames) = 0;
    virtual QString selectOneWhereId() = 0;
};

/*
   TODO:
   - Caching when save() and select() is called in sequence.
*/
class QormDefaultSessionIT : public QObject 
{
    Q_OBJECT

private:
    QormSessionFactory::Ptr m_sessionFactory;
    SqlHelper::Ptr m_sqlHelper;

private:
    virtual QormDatabaseFactory::Ptr databaseFactory() const = 0;
    virtual SqlQueries::Ptr sqlQueries() const = 0;
    virtual Mappings::Ptr mappings() const = 0;

public:
    QormDefaultSessionIT() { }
    virtual ~QormDefaultSessionIT() { }

private Q_SLOTS:
    void init()
    {
        auto dbFactory = databaseFactory();
        m_sessionFactory = QormSessionFactoryBuilder::AFactory()
            .withDatabase(dbFactory)
            .build();
        m_sqlHelper = SqlHelper::factory(dbFactory);
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
        session->save(person, mappings()->personSimpleMapping());
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 1);
        QCOMPARE(records[0][0].toInt(), person->getId());
        QCOMPARE(records[0][1].toString(), person->getName());
        QCOMPARE(records[0][2].toString(), person->getSurname());
    }

    void save_newPersonWithCustomMapping_onlyFieldsOfThatMappingArePersistet()
    {
        auto metaEntity = mappings()->personMappingWithSetupSourceAndSequence()
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
        session->save(person, mappings()->personComplexMapping());
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
        session->save(person, mappings()->personSimpleMapping());
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
        session->save(
            QormSql("insert into person (id, name, surname) values (1, ?, ?)", QVariantList() << name << surname));
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
        session->remove(person, mappings()->personSimpleMapping());
        session->commit();

        auto records = m_sqlHelper->select("select id, name, surname from person");
        QCOMPARE(records.size(), 0);
    }

    void selectOne_sqlThatReturnsNoResult_nullptr()
    {
        auto session = m_sessionFactory->createSession();

        auto item = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne()),
            mappings()->personSimpleMapping());

        QVERIFY(item==nullptr);
    }

    void selectOne_sqlThatReturnsOneResult_thatPerson()
    {
        QString name = "Mueller", surname = "Hans";
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << name << surname);

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne()),
            mappings()->personSimpleMapping());

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
            session->selectOne<PersonSimple>(QormSql("select * from person"), mappings()->personSimpleMapping()),
            QormMoreThanOneResultException);
    }

    void selectOne_sqlThatSelectsOnlyName_onePersonWithNameOnlyIsFilled()
    {
        QString name = "Mueller";
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << name << "Hans");

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "name")),
            mappings()->personSimpleMapping());

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
        auto item = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "id")),
            mappings()->personSimpleMapping());
        auto item2 = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "id")),
            mappings()->personSimpleMapping());

        QCOMPARE(item, item2);
    }

    void selectOne_sqlThatReturnsBothTimesTheSameResultWithName_bothResultsShouldDifferentBecauseOfNoCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "name")),
            mappings()->personSimpleMapping());
        auto item2 = session->selectOne<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "name")),
            mappings()->personSimpleMapping());

        QVERIFY(item!=item2);
    }

    void selectOne_sqlThatReturnsAnResultByAConcreteId_thePersonWithThatId()
    {
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(
            QormSql(sqlQueries()->selectOneWhereId(), QVariantList() << id),
            mappings()->personSimpleMapping());

        QVERIFY(item!=nullptr);
        QCOMPARE(item->getId(), id);
    }

    void selectOne_noPersonExists_nullptr()
    {
        auto session = m_sessionFactory->createSession();

        auto item = session->selectOne<PersonSimple>(1, mappings()->personSimpleMapping());

        QVERIFY(item==nullptr);
    }

    void selectOne_onePersonIsSelected_thatPerson()
    {
        QString name = "Mueller", surname = "Hans";
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << name << surname);

        auto session = m_sessionFactory->createSession();
        auto item = session->selectOne<PersonSimple>(id, mappings()->personSimpleMapping());

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
        auto item = session->selectOne<PersonSimple>(id, mappings()->personSimpleMapping());
        auto item2 = session->selectOne<PersonSimple>(id, mappings()->personSimpleMapping());

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
        auto item = session->selectOne<PersonComplex>(idPerson, mappings()->personComplexMapping());

        QVERIFY(item!=nullptr && item->getAddress()!=nullptr);
        QCOMPARE(item->getAddress()->getId(), idAddress);
        QCOMPARE(item->getAddress()->getCountry(), country);
        QCOMPARE(item->getAddress()->getPostCode(), postCode);
        QCOMPARE(item->getAddress()->getStreet(), street);
    }

    void selectOne_mappingWithConverter_valueGetsConverted()
    {
        auto mapping = mappings()->personMappingWithSetupSourceAndSequence()
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
            mappings()->personSimpleMapping());

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
            mappings()->personSimpleMapping());

        QCOMPARE(items.size(), 2);
    }

    void selectManyBySql_selectOnePersonWithIdTwoTimes_bothVersionsAreIdenticalDueToCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "id")),
            mappings()->personSimpleMapping());
        auto items2 = session->selectMany<PersonSimple>(QormSql(sqlQueries()->selectOne(QStringList() << "id")),
            mappings()->personSimpleMapping());

        QCOMPARE(items[0], items2[0]);
    }

    void selectManyBySql_onePersonIsSelectedById_thePersonWithThatId()
    {
        int id = 1;
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << id << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(
            QormSql(sqlQueries()->selectOneWhereId(), QVariantList() << id),
            mappings()->personSimpleMapping());

        QVERIFY(items.size()==1);
        QCOMPARE(items[0]->getId(), id);
    }

    void selectMany_noPersons_emptyList()
    {
        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(mappings()->personSimpleMapping());

        QVERIFY(items.isEmpty());
    }

    void selectMany_select2Persons_listOf2Persons()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 2 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(mappings()->personSimpleMapping());

        QCOMPARE(items.size(), 2);
    }

    void selectMany_selectOnePersonWithIdTwoTimes_bothVersionsAreIdenticalDueToCaching()
    {
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << AnyBuilder::anyString() << AnyBuilder::anyString());

        auto session = m_sessionFactory->createSession();
        auto items = session->selectMany<PersonSimple>(mappings()->personSimpleMapping());
        auto items2 = session->selectMany<PersonSimple>(mappings()->personSimpleMapping());

        QCOMPARE(items[0], items2[0]);
    }

    void selectManyWithCustomMapping_sqlWithIdAndNameAndNoPrefix_aPersonWithIdAndName()
    {
        QString name = "Mueller", surname = "Hans";
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << name << surname);

        auto person = PersonSimple::Ptr();
        auto session = m_sessionFactory->createSession();
        session->selectManyWithCustomMapping(QormSql("select id, name from person"), [&](const QormOnDemandRecordMapper *mapper) {
            person = mapper->mapToEntity<PersonSimple>(mappings()->personSimpleMapping());
            return false;
        });

        QVERIFY(person != nullptr);
        QCOMPARE(person->getId(), 1);
        QCOMPARE(person->getName(), name);
    }

    void selectManyWithCustomMapping_sqlWithIdAndNameAndAPrefix_aPersonWithIdAndName()
    {
        QString name = "Mueller", surname = "Hans";
        m_sqlHelper->insert("insert into person (id, name, surname) values (?,?,?)",
            QVariantList() << 1 << name << surname);

        auto person = PersonSimple::Ptr();
        auto session = m_sessionFactory->createSession();
        session->selectManyWithCustomMapping(QormSql("select p.id as p_id, p.name as p_name from person p"), [&](const QormOnDemandRecordMapper *mapper) {
            person = mapper->mapToEntity<PersonSimple>(mappings()->personSimpleMapping(), "p");
            return false;
        });

        QVERIFY(person != nullptr);
        QCOMPARE(person->getId(), 1);
        QCOMPARE(person->getName(), name);
    }

    void selectManyWithCustomMapping_sqlWithIdAndNameAndAddress_aPersonWithIdAndName()
    {
        int idAddress = 1;
        QString country = AnyBuilder::anyString(), postCode = AnyBuilder::anyString(), street = AnyBuilder::anyString();
        m_sqlHelper->insert("insert into address (id, country, postCode, street) values (?,?,?,?)",
            QVariantList() << idAddress << country << postCode << street);

        QString name = "Mueller", surname = "Hans";
        m_sqlHelper->insert("insert into person (id, name, surname, address) values (?,?,?,?)",
            QVariantList() << 1 << name << surname << idAddress);

        auto person = PersonSimple::Ptr();
        auto address = Address::Ptr();
        auto session = m_sessionFactory->createSession();
        session->selectManyWithCustomMapping(QormSql("select p.id as p_id, p.name as p_name, a.id as a_id from person p left join address a on (a.id = p.address)"), [&](const QormOnDemandRecordMapper *mapper) {
            person = mapper->mapToEntity<PersonSimple>(mappings()->personSimpleMapping(), "p");
            address = mapper->mapToEntity<Address>(mappings()->addressMapping(), "a");
            return false;
        });

        QVERIFY(person != nullptr);
        QCOMPARE(person->getId(), 1);
        QCOMPARE(person->getName(), name);

        QVERIFY(address != nullptr);
        QCOMPARE(address->getId(), idAddress);
    }
};
