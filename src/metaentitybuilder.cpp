#include <QMetaOrm/metaentitybuilder.h>

using namespace QMetaOrm;

MetaEntityBuilder MetaEntityBuilder::anEntity() {
   return MetaEntityBuilder();
}

MetaEntityBuilder::MetaEntityBuilder()
   : m_entity(MetaEntity::factory())
{
}

MetaEntityBuilder MetaEntityBuilder::forSource(const QString &source) {
   m_entity->setSource(source);
   return *this;
}

MetaEntityBuilder MetaEntityBuilder::withSequence(const QString &sequence) {
   m_entity->setSequence(sequence);
   return *this;
}

MetaEntityBuilder MetaEntityBuilder::withId(const QString &prop, const QString &field) {
   m_entity->setKey(prop, field);
   return *this;
}

MetaEntityBuilder MetaEntityBuilder::withData(const QString &prop, const QString &field, const QString &converter) {
   MetaProperty value;
   value.propertyName = prop;
   value.databaseName = field;
   value.converterName = converter;
   m_entity->addProperty(value);
   return *this;
}
