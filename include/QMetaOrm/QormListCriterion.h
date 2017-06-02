#pragma once

#include <QMetaOrm/QormCriterion.h>
#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QVariantList>
#include <QStringList>

class QMETAORM_LIBRARY_API QormListCriterion : public QormCriterion {
public:
   typedef QSharedPointer<QormListCriterion> Ptr;

   enum class ExpressionType {
      In
   };

public:
   static QormListCriterion::Ptr in(const QString &prop, const QVariantList &values);

   virtual QString dump() const override;

   virtual QString stringify(
      std::function<QString(const QormCriterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
      std::function<QString(const class QormValueCriterion *)> valueDelegate,
      std::function<QString(const class QormListCriterion *)> listDelegate) const override;

public:
   ExpressionType expressiontype;
   QString prop;
   QList<QVariant> values;

private:
   QormListCriterion(ExpressionType atype, const QString &aprop, const QVariantList &avalues);
};
