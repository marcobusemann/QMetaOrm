#include <QMetaOrm/QormFirebirdSqlQueryBuilder.h>

QString QormFirebirdSqlQueryBuilder::buildSequenceSelect(QormMetaEntity::Ptr mapping)
{
    Q_ASSERT_X(mapping->hasSequence(), __FUNCTION__, "actually inserting entities requires a sequence");
    return QString("SELECT NEXT VALUE FOR %1 FROM RDB$DATABASE;")
        .arg(mapping->getSequence());
}