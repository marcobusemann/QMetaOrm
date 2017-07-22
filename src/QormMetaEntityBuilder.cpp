#include <QMetaOrm/QormMetaEntityBuilder.h>

QormMetaEntityBuilder QormMetaEntityBuilder::anEntity()
{
    return QormMetaEntityBuilder();
}

QormMetaEntityBuilder QormMetaEntityBuilder::derivedEntity(const QormMetaEntity::Ptr& aParentEntity)
{
    return QormMetaEntityBuilder(aParentEntity);
}

QormMetaEntityBuilder::QormMetaEntityBuilder(const QormMetaEntity::Ptr& aParentEntity)
    :m_entity(aParentEntity->copy())
{
}

QormMetaEntityBuilder::QormMetaEntityBuilder()
    :m_entity(QormMetaEntity::factory())
{
}

QormMetaEntityBuilder QormMetaEntityBuilder::forSource(const QString& source)
{
    m_entity->setSource(source);
    return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withSequence(const QString& sequence)
{
    m_entity->setSequence(sequence);
    return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withId(const QString& prop, const QString& field)
{
    m_entity->setKey(prop, field);
    return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withData(const QString& prop, const QString& field)
{
    QormMetaProperty value;
    value.propertyName = prop;
    value.databaseName = field;
    m_entity->addProperty(value);
    return *this;
}

QormMetaEntityBuilder
QormMetaEntityBuilder::withConvertedData(const QString& prop, const QString& field,
    std::function<QormConverter::Ptr()> converterSelector)
{
    QormMetaProperty value;
    value.propertyName = prop;
    value.databaseName = field;
    value.converterSelector = converterSelector;
    m_entity->addProperty(value);
    return *this;
}

QormMetaEntityBuilder
QormMetaEntityBuilder::withConvertedData(const QString& prop, const QString& field, const QormConverter::Ptr& converter)
{
    return withConvertedData(prop, field, [converter]() {
        return converter;
    });
}

QormMetaEntityBuilder
QormMetaEntityBuilder::withOneToMany(const QString& prop, const QString& field, QormMetaEntity::Ptr referenceEntity)
{
    QormMetaProperty value;
    value.propertyName = prop;
    value.databaseName = field;
    value.reference = referenceEntity;
    m_entity->addProperty(value);
    return *this;
}

QormMetaEntityBuilder QormMetaEntityBuilder::withEntityFactory(const QormEntityFactory::Ptr& entityFactory)
{
    m_entity->setEntityFactory(entityFactory);
    return *this;
}
