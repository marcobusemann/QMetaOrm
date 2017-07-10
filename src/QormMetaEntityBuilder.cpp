#include <QMetaOrm/QormMetaEntityBuilder.h>

QormMetaEntityBuilder QormMetaEntityBuilder::anEntity() {
   return QormMetaEntityBuilder();
}

QormMetaEntityBuilder QormMetaEntityBuilder::derivedEntity(const QormMetaEntity::Ptr& aParentEntity)
{
   return QormMetaEntityBuilder(aParentEntity);
}

QormMetaEntityBuilder::QormMetaEntityBuilder(const QormMetaEntity::Ptr& aParentEntity)
   : m_entity(aParentEntity->copy())
   , m_useEmbeddedPtrEntityFactoryNamingScheme(false)
{
}

QormMetaEntityBuilder::QormMetaEntityBuilder()
   : m_entity(QormMetaEntity::factory())
   , m_useEmbeddedPtrEntityFactoryNamingScheme(false)
{
}

QormMetaEntityBuilder QormMetaEntityBuilder::forSource(const QString &source) {
   m_entity->setSource(source);
   return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withSequence(const QString &sequence) {
   m_entity->setSequence(sequence);
   return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withId(const QString &prop, const QString &field) {
   m_entity->setKey(prop, field);
   return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withData(const QString &prop, const QString &field, const QString &converter) {
   QormMetaProperty value;
   value.propertyName = prop;
   value.databaseName = field;
   value.converterName = converter;
   m_entity->addProperty(value);
   return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withOneToMany(const QString & prop, const QString & field, QormMetaEntity::Ptr referenceEntity)
{
   QormMetaProperty value;
   value.propertyName = prop;
   value.databaseName = field;
   value.reference = referenceEntity;
   m_entity->addProperty(value);
   return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withEntityFactory(const QormEntityFactory::Ptr &entityFactory)
{
   m_entity->setEntityFactory(entityFactory);
   return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withEmbeddedPtrNamingScheme()
{
   m_useEmbeddedPtrEntityFactoryNamingScheme = true;
   return *this;
}
