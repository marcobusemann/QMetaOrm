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
   template <> QormMetaEntity::Ptr mapping<PersonSimple>() {
      static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
         .forSource("person")
         .withId(PersonSimple::p::id, "ID")
         .withData(PersonSimple::p::name, "NAME")
         .withData(PersonSimple::p::surname, "SURNAME")
         .build<PersonSimple>();
      return map;
   }

   template <> QormMetaEntity::Ptr mapping<Address>() {
      static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
         .forSource("address")
         .withId(Address::p::id, "ID")
         .withData(Address::p::country, "COUNTRY")
         .withData(Address::p::postCode, "POSTCODE")
         .withData(Address::p::street, "STREET")
         .build<Address>();
      return map;
   }

   template <> QormMetaEntity::Ptr mapping<PersonComplex>() {
      static const QormMetaEntity::Ptr map = QormMetaEntityBuilder::anEntity()
         .forSource("person")
         .withId(PersonComplex::p::id, "ID")
         .withData(PersonComplex::p::name, "NAME")
         .withData(PersonComplex::p::surname, "SURNAME")
         .withOneToMany(PersonComplex::p::address, "ADDRESS", mapping<Address>())
         .build<PersonComplex>();
      return map;
   }
}
