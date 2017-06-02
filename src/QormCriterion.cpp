#include <QMetaOrm/QormCriterion.h>

QormCriterion::Ptr QormCriterion::andCondition(QormCriterion::Ptr left, QormCriterion::Ptr right) {
   if (!left && right)
      return right;
   else if (!right && left)
      return left;
   return QormCriterion::Ptr(new QormCriterion(CombinationType::And, left, right));
}

QormCriterion::Ptr QormCriterion::orCondition(QormCriterion::Ptr left, QormCriterion::Ptr right) {
   if (!left && right)
      return right;
   else if (!right && left)
      return left;
   return QormCriterion::Ptr(new QormCriterion(CombinationType::Or, left, right));
}

QString QormCriterion::dump() const {
   return QString("((%1) %2 (%3))")
      .arg(left ? left->dump() : "")
      .arg(combinationtype == CombinationType::Leaf ? "_" :
         combinationtype == CombinationType::And ? " & " :
         combinationtype == CombinationType::Or ? " | " : "")
      .arg(right ? right->dump() : "");
}

QString QormCriterion::stringify(
   std::function<QString(const QormCriterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
   std::function<QString(const class QormValueCriterion *)> valueDelegate,
   std::function<QString(const class QormListCriterion *)> listDelegate) const {
   QString leftString = left->stringify(containerDelegate, valueDelegate, listDelegate);
   QString rightString = right->stringify(containerDelegate, valueDelegate, listDelegate);
   return containerDelegate(this, leftString, rightString);
}

QormCriterion::QormCriterion(CombinationType atype, QormCriterion::Ptr aleft, QormCriterion::Ptr aright)
   : combinationtype(atype)
   , left(aleft)
   , right(aright)
{
}

QormCriterion::QormCriterion()
   : combinationtype(CombinationType::Leaf)
{
}
