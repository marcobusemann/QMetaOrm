#include <QMetaOrm/metaentitybuilder.h>

using namespace QMetaOrm;

//-----------------------------------------------------------------------------
MetaEntityBuilder MetaEntityBuilder::anEntity() {
   return MetaEntityBuilder();
}

//-----------------------------------------------------------------------------
MetaEntityBuilder MetaEntityBuilder::forSource(const QString &source) {
   m_entity.source = source;
   return *this;
}

//-----------------------------------------------------------------------------
MetaEntityBuilder MetaEntityBuilder::withSequence(const QString &sequence) {
   m_entity.sequence = sequence;
   return *this;
}

//-----------------------------------------------------------------------------
MetaEntityBuilder MetaEntityBuilder::withId(const QString &prop, const QString &field) {
   m_entity.key = qMakePair(prop, field);
   return *this;
}

//-----------------------------------------------------------------------------
MetaEntityBuilder MetaEntityBuilder::withData(const QString &prop, const QString &field, const QString &converter) {
   MetaProperty value;
   value.propertyName = prop;
   value.databaseName = field;
   value.converterName = converter;
   m_entity.propertyMapping[prop] = value;
   return *this;
}
