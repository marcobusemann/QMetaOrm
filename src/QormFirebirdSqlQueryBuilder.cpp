#include <QMetaOrm/QormFirebirdSqlQueryBuilder.h>

QString QormFirebirdSqlQueryBuilder::buildSequenceSelect(const QString &aSequence)
{
    return QString("SELECT NEXT VALUE FOR %1 FROM RDB$DATABASE;")
        .arg(aSequence);
}