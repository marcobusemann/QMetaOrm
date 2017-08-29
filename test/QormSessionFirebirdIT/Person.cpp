#include <QMetaOrm/QormMetaEntityBuilder.h>

#include "Person.h"

const QString PersonSimple::p::id = "id";
const QString PersonSimple::p::name = "name";
const QString PersonSimple::p::surname = "surname";

const QString PersonComplex::p::id = "id";
const QString PersonComplex::p::name = "name";
const QString PersonComplex::p::surname = "surname";
const QString PersonComplex::p::address = "address";

const QString Address::p::id = "id";
const QString Address::p::country = "country";
const QString Address::p::postCode = "postCode";
const QString Address::p::street = "street";

namespace QormMappings {
    QormMetaEntity::Ptr TsPersonSimpleMapping()
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

    QormMetaEntity::Ptr TsAddressMapping()
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

    QormMetaEntity::Ptr TsPersonComplexMapping()
    {
        static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
            .forSource("person")
            .withSequence("person_gen")
            .withId(PersonComplex::p::id, "ID")
            .withData(PersonComplex::p::name, "NAME")
            .withData(PersonComplex::p::surname, "SURNAME")
            .withOneToMany(PersonComplex::p::address, "ADDRESS", TsAddressMapping())
            .build<PersonComplex>();
        return map;
    }
}
