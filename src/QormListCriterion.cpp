#include <QMetaOrm/QormListCriterion.h>

QormListCriterion::Ptr QormListCriterion::in(const QString &prop, const QVariantList &values) {
   return QormListCriterion::Ptr(new QormListCriterion(ExpressionType::In, prop, values));
}

QString QormListCriterion::dump() const {
   QStringList stringifiedValues;
   foreach(QVariant value, values)
      stringifiedValues.append(value.toString());

   return QString(" %1 %2 %3 ")
      .arg(prop)
      .arg(expressiontype == ExpressionType::In ? " in " : "")
      .arg(stringifiedValues.join(","));
}

QString QormListCriterion::stringify(
   std::function<QString(const QormCriterion *, const QString & /*leftChild*/, const QString &rightChild)> /*containerDelegate*/,
   std::function<QString(const class QormValueCriterion *)> /*valueDelegate*/,
   std::function<QString(const class QormListCriterion *)> listDelegate) const {
   return listDelegate(this);
}

QormListCriterion::QormListCriterion(ExpressionType atype, const QString &aprop, const QVariantList &avalues)
   : expressiontype(atype)
   , prop(aprop)
   , values(avalues)
{}
