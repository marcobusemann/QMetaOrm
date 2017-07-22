#pragma once

#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QVariant>

class QMETAORM_LIBRARY_API QormConverter {
public:
    typedef QSharedPointer<QormConverter> Ptr;

public:
    virtual ~QormConverter() { }

    virtual QVariant convert(const QVariant& value) const = 0;
};
