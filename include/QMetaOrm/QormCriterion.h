#pragma once

#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>

#include <functional>

class QMETAORM_LIBRARY_API QormCriterion
{
public:
   typedef QSharedPointer<QormCriterion> Ptr;

   enum class CombinationType {
      Leaf,
      And,
      Or
   };

public:
   virtual ~QormCriterion() {}

   static QormCriterion::Ptr andCondition(QormCriterion::Ptr left, QormCriterion::Ptr right = QormCriterion::Ptr());
   static QormCriterion::Ptr orCondition(QormCriterion::Ptr left, QormCriterion::Ptr right = QormCriterion::Ptr());

   virtual QString dump() const;

   virtual QString stringify(
      std::function<QString(const QormCriterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
      std::function<QString(const class QormValueCriterion *)> valueDelegate,
      std::function<QString(const class QormListCriterion *)> listDelegate) const;

public:
   CombinationType combinationtype;
   QormCriterion::Ptr left;
   QormCriterion::Ptr right;

protected:
   QormCriterion(CombinationType atype, QormCriterion::Ptr aleft, QormCriterion::Ptr aright);
   QormCriterion();
};
