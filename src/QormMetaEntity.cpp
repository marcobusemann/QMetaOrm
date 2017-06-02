#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormExceptions.h>

const QList<QVariant::Type> QormMetaEntity::SupportedKeyTypes = (QList<QVariant::Type>() << QVariant::Int << QVariant::LongLong << QVariant::String);

struct MetaEntityBasicMetaTypeRegistration {
   MetaEntityBasicMetaTypeRegistration() {
      qRegisterMetaType<QSharedPointer<QObject>>();
   }
} metaEntityBasicMetaTypeRegistration;

QStringList QormMetaEntity::getDatabaseFields() const
{
   QStringList result;
   result << getKeyDatabaseField();
   foreach(auto value, m_propertyMapping)
      result << value.databaseName;
   return result;
}

QormMetaEntity::Ptr QormMetaEntity::copy()
{
   return Ptr(new QormMetaEntity(*this));
}

QormMetaEntity::QormMetaEntity()
   : m_keyGenerationstrategy(KeyGenerationStrategy::Identity)
{
}

QormMetaEntity::QormMetaEntity(const QormMetaEntity &rhs)
{
   m_source = rhs.m_source;
   m_sequence = rhs.m_sequence;
   m_key = rhs.m_key;
   m_propertyMapping = rhs.m_propertyMapping;
   m_entityFactory = rhs.m_entityFactory;
   m_keyGenerationstrategy = rhs.m_keyGenerationstrategy;
}

void QormMetaEntity::setSource(const QString & aSource)
{
   m_source = aSource;
}

const QString &QormMetaEntity::getSource() const
{
   return m_source;
}

bool QormMetaEntity::hasSource() const
{
   return !m_source.isEmpty();
}

void QormMetaEntity::setSequence(const QString &aSequence)
{
   m_sequence = aSequence;
   m_keyGenerationstrategy = KeyGenerationStrategy::Sequence;
}

const QString &QormMetaEntity::getSequence() const
{
   return m_sequence;
}

bool QormMetaEntity::hasSequence() const
{
   return !m_sequence.isEmpty();
}

KeyGenerationStrategy QormMetaEntity::getKeyGenerationStrategy() const
{
   return m_keyGenerationstrategy;
}

void QormMetaEntity::setKey(const QString &aProperty, const QString &aDatabaseField)
{
   m_key = qMakePair(aProperty, aDatabaseField);
}

const QString &QormMetaEntity::getKeyProperty() const
{
   return m_key.first;
}

const QString &QormMetaEntity::getKeyDatabaseField() const
{
   return m_key.second;
}

bool QormMetaEntity::hasKey() const
{
   return !m_key.first.isEmpty() && !m_key.second.isEmpty();
}

QList<QString> QormMetaEntity::getProperties() const
{
   return m_propertyMapping.keys();
}

const QormMetaProperty &QormMetaEntity::getProperty(const QString &aProperty)
{
   return m_propertyMapping[aProperty];
}

void QormMetaEntity::addProperty(const QormMetaProperty &prop)
{
   m_propertyMapping[prop.propertyName] = prop;
}

QList<QormMetaProperty> QormMetaEntity::getReferences() const
{
   QList<QormMetaProperty> references;
   for (auto prop : m_propertyMapping) {
      if (prop.isReference())
         references.append(prop);
   }
   return references;
}

QormEntityFactory::Ptr QormMetaEntity::getEntityFactory() const
{
   return m_entityFactory;
}

void QormMetaEntity::setEntityFactory(const QormEntityFactory::Ptr &entityFactory)
{
   m_entityFactory = entityFactory;
}
