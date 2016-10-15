#include <QMetaOrm/metaentity.h>

using namespace QMetaOrm;

const QList<QVariant::Type> MetaEntity::SupportedKeyTypes = (QList<QVariant::Type>() << QVariant::Int << QVariant::LongLong << QVariant::String);

//-----------------------------------------------------------------------------
QStringList MetaEntity::getDatabaseFields() const
{
   QStringList result;
   result << key.second;
   foreach(auto value, propertyMapping)
      result << value.databaseName;
   return result;
}
