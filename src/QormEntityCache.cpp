#include "QormEntityCache.h"

QVariant QormEntityCache::get(const QVariant& key, const QormMetaEntity::Ptr& mapping)
{
    return forMapping(mapping)[key];
}

bool QormEntityCache::contains(const QVariant& key, const QormMetaEntity::Ptr& mapping)
{
    return forMapping(mapping).contains(key);
}

void QormEntityCache::put(const QVariant& key, const QVariant& item, const QormMetaEntity::Ptr& mapping)
{
    forMapping(mapping)[key] = item;
}

QHash<QVariant, QVariant>& QormEntityCache::forMapping(const QormMetaEntity::Ptr& mapping)
{
    if (!m_data.contains(mapping))
        m_data[mapping] = QHash<QVariant, QVariant>();
    return m_data[mapping];
}

uint qHash(const QVariant& var)
{
    if (!var.isValid() || var.isNull())
        Q_ASSERT(0);

    switch (var.type()) {
    case QVariant::Int:
        return qHash(var.toInt());
        break;
    case QVariant::UInt:
        return qHash(var.toUInt());
        break;
    case QVariant::Bool:
        return qHash(var.toUInt());
        break;
    case QVariant::Double:
        return qHash(var.toUInt());
        break;
    case QVariant::LongLong:
        return qHash(var.toLongLong());
        break;
    case QVariant::ULongLong:
        return qHash(var.toULongLong());
        break;
    case QVariant::String:
        return qHash(var.toString());
        break;
    case QVariant::Char:
        return qHash(var.toChar());
        break;
    case QVariant::StringList:
        return qHash(var.toString());
        break;
    case QVariant::ByteArray:
        return qHash(var.toByteArray());
        break;
    case QVariant::Date:
    case QVariant::Time:
    case QVariant::DateTime:
    case QVariant::Url:
    case QVariant::Locale:
    case QVariant::RegExp:
        return qHash(var.toString());
        break;
    case QVariant::Map:
    case QVariant::List:
    case QVariant::BitArray:
    case QVariant::Size:
    case QVariant::SizeF:
    case QVariant::Rect:
    case QVariant::LineF:
    case QVariant::Line:
    case QVariant::RectF:
    case QVariant::Point:
    case QVariant::PointF:
        // not supported yet
        break;
    case QVariant::UserType:
    case QVariant::Invalid:
    default:
        return -1;
    }

    // could not generate a hash for the given variant
    return -1;
}
