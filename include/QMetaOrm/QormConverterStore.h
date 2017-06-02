#pragma once

#include <QMetaOrm/QormConverter.h>
#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>

class QMETAORM_LIBRARY_API QormConverterStore
{
public:
   typedef QSharedPointer<QormConverterStore> Ptr;

public:
   virtual ~QormConverterStore() {}
   virtual void registerConverter(const QString &name, const QormConverter::Ptr &converter) = 0;
   virtual QormConverter::Ptr getConverterFor(const QString &name) const = 0;
   virtual bool hasConverter(const QString &name) const = 0;
};

class QMETAORM_LIBRARY_API QormDefaultConverterStore : public QormConverterStore
{
public:
   typedef QSharedPointer<QormDefaultConverterStore> Ptr;
   static Ptr factory();

public:
   virtual void registerConverter(const QString &name, const QormConverter::Ptr &converter) override;
   virtual QormConverter::Ptr getConverterFor(const QString &name) const override;
   virtual bool hasConverter(const QString &name) const override;

private:
   QHash<QString, QormConverter::Ptr> m_converters;
};
