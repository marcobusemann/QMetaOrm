#include "QormDefaultSessionIT.h"
#include "DatabaseFactory.h"

class SqlSqliteQueries : public SqlQueries
{
    QString selectOne() override
    {
        return QString("select * from person limit 1");
    }

    QString selectOne(const QStringList &aColumnNames) override
    {
        return QString("select %1 from person limit 1").arg(aColumnNames.join(";"));
    }

    QString selectOneWhereId() override
    {
        return QString("select * from person where id = ? limit 1");
    }
};

class SqliteMappings : public Mappings
{
    QormMetaEntityBuilder personMappingWithSetupSourceAndSequence() override
    {
        static const auto builder = QormMetaEntityBuilder::anEntity()
            .forSource("person");
        return builder;
    }

    QormMetaEntity::Ptr personSimpleMapping() override
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withId(PersonSimple::p::id, "ID")
            .withData(PersonSimple::p::name, "NAME")
            .withData(PersonSimple::p::surname, "SURNAME")
            .build<PersonSimple>();
        return map;
    }

    QormMetaEntity::Ptr addressMapping() override
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("address")
            .withId(Address::p::id, "ID")
            .withData(Address::p::country, "COUNTRY")
            .withData(Address::p::postCode, "POSTCODE")
            .withData(Address::p::street, "STREET")
            .build<Address>();
        return map;
    }

    QormMetaEntity::Ptr personComplexMapping() override
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withId(PersonComplex::p::id, "ID")
            .withData(PersonComplex::p::name, "NAME")
            .withData(PersonComplex::p::surname, "SURNAME")
            .withOneToMany(PersonComplex::p::address, "ADDRESS", addressMapping())
            .build<PersonComplex>();
        return map;
    }
};

class QormDefaultSessionSqliteIT : public QormDefaultSessionIT
{
    Q_OBJECT
private:

    virtual QormDatabaseFactory::Ptr databaseFactory() const override
    {
        return SQLiteEmbeddedDatabaseFactory::factory();
    }

    virtual SqlQueries::Ptr sqlQueries() const override
    {
        return SqlQueries::Ptr(new SqlSqliteQueries());
    }

    virtual Mappings::Ptr mappings() const override
    {
        return Mappings::Ptr(new SqliteMappings());
    }
};

QTEST_APPLESS_MAIN(QormDefaultSessionSqliteIT)

#include "QormDefaultSessionSqliteIT.moc"
