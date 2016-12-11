#include <QMetaOrm/ValueCriterion.h>

using namespace QMetaOrm;

ValueCriterion::Ptr ValueCriterion::equals(const QString &prop, const QVariant &value) {
   return ValueCriterion::Ptr(new ValueCriterion(ExpressionType::Equals, prop, value));
}

QString ValueCriterion::dump() const {
   return QString("%1 %2 %3")
      .arg(prop)
      .arg(expressiontype == ExpressionType::Equals ? " = " : "")
      .arg(value.toString());
}

QString ValueCriterion::stringify(
   std::function<QString(const Criterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
   std::function<QString(const class ValueCriterion *)> valueDelegate,
   std::function<QString(const class ListCriterion *)> listDelegate) const {
   return valueDelegate(this);
}

ValueCriterion::ValueCriterion(ExpressionType atype, const QString &aprop, const QVariant &avalue)
   : value(avalue)
   , prop(aprop)
   , expressiontype(atype)
{}
