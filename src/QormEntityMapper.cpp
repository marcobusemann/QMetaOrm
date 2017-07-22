#include "QormEntityMapper.h"

QormPropertyPrefixer::Handler
QormPropertyPrefixer::getRecordValuePrefixer(const QSqlRecord& record, const QString& prefixString)
{
    return [record, prefixString, this](const QString& prop) -> QVariant {
        return record.value(prepend(prop, prefixString));
    };
}

QormPropertyPrefixer::Handler
QormPropertyPrefixer::getEmbeddedRecordValuePrefixer(Handler prefixer, const QString& prefix)
{
    return [prefixer, prefix, this](const QString& prop) -> QVariant {
        return prefixer(prepend(prop, prefix));
    };
}

QString QormPropertyPrefixer::prepend(const QString& value, const QString& prefixString)
{
    return prefixString.isEmpty() ? value : QString("%1_%2").arg(prefixString, value);
}

QormEntityMapper::QormEntityMapper(const QormLogger::Ptr& logger, const QormEntityCache::Ptr& entityCache)
    :m_logger(logger), m_entityCache(entityCache)
{
}
