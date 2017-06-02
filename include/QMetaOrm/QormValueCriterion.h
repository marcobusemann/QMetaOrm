#pragma once

#include <QMetaOrm/QormCriterion.h>
#include <QMetaOrm/QormPrivate.h>

#include <QVariant>

class QMETAORM_LIBRARY_API QormValueCriterion : public QormCriterion {
public:
   typedef QSharedPointer<QormValueCriterion> Ptr;

   enum class ExpressionType {
      Equals
   };

public:
   static QormValueCriterion::Ptr equals(const QString &prop, const QVariant &value);

   virtual QString dump() const override;
   virtual QString stringify(
      std::function<QString(const QormCriterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
      std::function<QString(const class QormValueCriterion *)> valueDelegate,
      std::function<QString(const class QormListCriterion *)> listDelegate) const override;

public:
   ExpressionType expressiontype;
   QString prop;
   QVariant value;

private:
   QormValueCriterion(ExpressionType atype, const QString &aprop, const QVariant &avalue);
};
