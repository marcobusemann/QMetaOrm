#include <QMetaOrm/ListCriterion.h>

using namespace QMetaOrm;

ListCriterion::Ptr ListCriterion::in(const QString &prop, const QVariantList &values) {
   return ListCriterion::Ptr(new ListCriterion(ExpressionType::In, prop, values));
}

QString ListCriterion::dump() const {
   QStringList stringifiedValues;
   foreach(QVariant value, values)
      stringifiedValues.append(value.toString());

   return QString(" %1 %2 %3 ")
      .arg(prop)
      .arg(expressiontype == ExpressionType::In ? " in " : "")
      .arg(stringifiedValues.join(","));
}

QString ListCriterion::stringify(
   std::function<QString(const Criterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
   std::function<QString(const class ValueCriterion *)> valueDelegate,
   std::function<QString(const class ListCriterion *)> listDelegate) const {
   return listDelegate(this);
}

ListCriterion::ListCriterion(ExpressionType atype, const QString &aprop, const QVariantList &avalues)
   : values(avalues)
   , prop(aprop)
   , expressiontype(atype)
{}
