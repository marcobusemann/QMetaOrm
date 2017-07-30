#pragma once

#include <QMetaOrm/QormPrivate.h>
#include <QMetaOrm/QormSession.h>

#include <QSharedPointer>

class QMETAORM_LIBRARY_API QormSessionFactory {
public:
    typedef QSharedPointer<QormSessionFactory> Ptr;

public:
    virtual ~QormSessionFactory() { }

    virtual QormSession::Ptr createSession() const = 0;
};

