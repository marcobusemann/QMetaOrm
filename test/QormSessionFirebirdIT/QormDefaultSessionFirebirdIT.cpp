#include "QormDefaultSessionIT.h"
#include "DatabaseFactory.h"

class SqlFirebirdQueries : public SqlQueries
{
    QString selectOne() override
    {
        return QString("select * from person rows 1");
    }

    QString selectOne(const QStringList &aColumnNames) override
    {
        return QString("select %1 from person rows 1").arg(aColumnNames.join(";"));
    }

    QString selectOneWhereId() override
    {
        return QString("select * from person where id = ? rows 1");
    }
};

class FirebirdMappings : public Mappings
{
    QormMetaEntityBuilder personMappingWithSetupSourceAndSequence()
    {
        static const auto builder = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withSequence("person_gen");
        return builder;
    }

    QormMetaEntity::Ptr personSimpleMapping()
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withSequence("person_gen")
            .withId(PersonSimple::p::id, "ID")
            .withData(PersonSimple::p::name, "NAME")
            .withData(PersonSimple::p::surname, "SURNAME")
            .build<PersonSimple>();
        return map;
    }

    QormMetaEntity::Ptr addressMapping()
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("address")
            .withSequence("address_gen")
            .withId(Address::p::id, "ID")
            .withData(Address::p::country, "COUNTRY")
            .withData(Address::p::postCode, "POSTCODE")
            .withData(Address::p::street, "STREET")
            .build<Address>();
        return map;
    }

    QormMetaEntity::Ptr personComplexMapping()
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withSequence("person_gen")
            .withId(PersonComplex::p::id, "ID")
            .withData(PersonComplex::p::name, "NAME")
            .withData(PersonComplex::p::surname, "SURNAME")
            .withOneToMany(PersonComplex::p::address, "ADDRESS", addressMapping())
            .build<PersonComplex>();
        return map;
    }
};

class QormDefaultSessionFirebirdIT : public QormDefaultSessionIT
{
    Q_OBJECT
private:
    virtual QormDatabaseFactory::Ptr databaseFactory() const override
    {
        return FirebirdDatabaseFactory::factory();
    }

    virtual SqlQueries::Ptr sqlQueries() const override
    {
        return SqlQueries::Ptr(new SqlFirebirdQueries());
    }

    virtual Mappings::Ptr mappings() const override
    {
        return Mappings::Ptr(new FirebirdMappings());
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionFirebirdIT)

#include "QormDefaultSessionFirebirdIT.moc"
