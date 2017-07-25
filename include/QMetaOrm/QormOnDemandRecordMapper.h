#pragma once

#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormPrivate.h>

class QMETAORM_LIBRARY_API QormOnDemandRecordMapper {
public:
    virtual ~QormOnDemandRecordMapper() { }

    template<class T>
    QSharedPointer<T> mapToEntity(const QormMetaEntity::Ptr& mapping, const QString& prefix = QString()) const
    {
        return mapToEntity(mapping, prefix).objectCast<T>();
    }

    virtual QSharedPointer<QObject> mapToEntity(const QormMetaEntity::Ptr& mapping,
                                                const QString& prefix = QString()) const = 0;
};