#include <QMetaOrm/EntityMapper.h>

using namespace QMetaOrm;

PropertyPrefixer::Handler PropertyPrefixer::getRecordValuePrefixer(const QSqlRecord & record, const QString & prefixString)
{
   return [record, prefixString, this](const QString &prop) -> QVariant {
      return record.value(prepend(prop, prefixString));
   };
}

PropertyPrefixer::Handler PropertyPrefixer::getEmbeddedRecordValuePrefixer(Handler prefixer, const QString & prefix)
{
   return [prefixer, prefix, this](const QString &prop) -> QVariant {
      return prefixer(prepend(prop, prefix));
   };
}

QString PropertyPrefixer::prepend(const QString & value, const QString & prefixString)
{
   return prefixString.isEmpty() ? value : QString("%1_%2").arg(prefixString, value);
}

EntityMapper::EntityMapper(const Logger::Ptr &logger, const EntityCache::Ptr &entityCache)
   : m_logger(logger)
   , m_entityCache(entityCache)
{
}