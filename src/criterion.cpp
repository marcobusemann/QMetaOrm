#include <QMetaOrm/criterion.h>

using namespace QMetaOrm;

Criterion::Ptr Criterion::and(Criterion::Ptr left,  Criterion::Ptr right) {
   if (!left && right)
      return right;
   else if (!right && left)
      return left;
   return Criterion::Ptr(new Criterion(CombinationType::And, left, right));
}

Criterion::Ptr Criterion::or(Criterion::Ptr left,  Criterion::Ptr right) {
   if (!left && right)
      return right;
   else if (!right && left)
      return left;
   return Criterion::Ptr(new Criterion(CombinationType::Or, left, right));
}

QString Criterion::dump() const {
   return QString("((%1) %2 (%3))")
      .arg(left ? left->dump() : "")
      .arg(combinationtype == CombinationType::Leaf ? "_" :
           combinationtype == CombinationType::And ? " & " :
           combinationtype == CombinationType::Or ? " | " : "")
      .arg(right ? right->dump() : "");
}

QString Criterion::stringify(
   std::function<QString(const Criterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
   std::function<QString(const class ValueCriterion *)> valueDelegate,
   std::function<QString(const class ListCriterion *)> listDelegate) const {
   QString leftString = left->stringify(containerDelegate, valueDelegate, listDelegate);
   QString rightString = right->stringify(containerDelegate, valueDelegate, listDelegate);
   return containerDelegate(this, leftString, rightString);
}

Criterion::Criterion(CombinationType atype, Criterion::Ptr aleft, Criterion::Ptr aright)
   : combinationtype(atype)
   , left(aleft)
   , right(aright)
{
}

Criterion::Criterion()
   : combinationtype(CombinationType::Leaf)
{
}
