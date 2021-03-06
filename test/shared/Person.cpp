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
