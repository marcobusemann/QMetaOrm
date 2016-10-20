#include <QMetaOrm/metaentity.h>

using namespace QMetaOrm;

const QList<QVariant::Type> MetaEntity::SupportedKeyTypes = (QList<QVariant::Type>() << QVariant::Int << QVariant::LongLong << QVariant::String);

QStringList MetaEntity::getDatabaseFields() const
{
   QStringList result;
   result << getKeyDatabaseField();
   foreach(auto value, m_propertyMapping)
      result << value.databaseName;
   return result;
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
