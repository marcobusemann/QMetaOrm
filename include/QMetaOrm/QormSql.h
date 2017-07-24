#pragma once

#include <QMetaOrm/QormPrivate.h>

#include <QVariantList>
#include <QString>

struct QMETAORM_LIBRARY_API QormSql {
    QString sql;
    QVariantList parameters;

    QormSql(const QString& sql, const QVariantList& parameters)
        :sql(sql)
         , parameters(parameters) { }

    QormSql(const QString& sql)
        :sql(sql) { }
};