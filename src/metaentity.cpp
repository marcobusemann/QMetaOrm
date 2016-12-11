#include <QMetaOrm/MetaEntity.h>
#include <QMetaOrm\Exceptions.h>

using namespace QMetaOrm;

const QList<QVariant::Type> MetaEntity::SupportedKeyTypes = (QList<QVariant::Type>() << QVariant::Int << QVariant::LongLong << QVariant::String);

struct MetaEntityBasicMetaTypeRegistration {
   MetaEntityBasicMetaTypeRegistration() {
      qRegisterMetaType<QSharedPointer<QObject>>();
   }
} metaEntityBasicMetaTypeRegistration;

QStringList MetaEntity::getDatabaseFields() const
{
   QStringList result;
   result << getKeyDatabaseField();
   foreach(auto value, m_propertyMapping)
      result << value.databaseName;
   return result;
}

MetaEntity::Ptr MetaEntity::copy()
{
   return Ptr(new MetaEntity(*this));
}

MetaEntity::MetaEntity(const MetaEntity &rhs)
{
   m_source = rhs.m_source;
   m_sequence = rhs.m_sequence;
   m_key = rhs.m_key;
   m_propertyMapping = rhs.m_propertyMapping;
   m_entityFactory = rhs.m_entityFactory;
}

void MetaEntity::setSource(const QString & aSource)
{
   m_source = aSource;
}

const QString &MetaEntity::getSource() const
{
   return m_source;
}

bool MetaEntity::hasSource() const
{
   return !m_source.isEmpty();
}

void MetaEntity::setSequence(const QString &aSequence)
{
   m_sequence = aSequence;
}

const QString &MetaEntity::getSequence() const
{
   return m_sequence;
}

bool MetaEntity::hasSequence() const
{
   return !m_sequence.isEmpty();
}

void MetaEntity::setKey(const QString &aProperty, const QString &aDatabaseField)
{
   m_key = qMakePair(aProperty, aDatabaseField);
}

const QString &MetaEntity::getKeyProperty() const
{
   return m_key.first;
}

const QString &MetaEntity::getKeyDatabaseField() const
{
   return m_key.second;
}

bool MetaEntity::hasKey() const
{
   return !m_key.first.isEmpty() && !m_key.second.isEmpty();
}

QList<QString> MetaEntity::getProperties() const
{
   return m_propertyMapping.keys();
}

const MetaProperty &MetaEntity::getProperty(const QString &aProperty)
{
   return m_propertyMapping[aProperty];
}

void MetaEntity::addProperty(const MetaProperty &prop)
{
   m_propertyMapping[prop.propertyName] = prop;
}

QList<MetaProperty> MetaEntity::getReferences() const
{
   QList<MetaProperty> references;
   for (auto prop : m_propertyMapping) {
      if (prop.isReference())
         references.append(prop);
   }
   return references;
}

EntityFactory::Ptr MetaEntity::getEntityFactory() const
{
   return m_entityFactory;
}

void MetaEntity::setEntityFactory(const EntityFactory::Ptr &entityFactory)
{
   m_entityFactory = entityFactory;
}
