#include <QMetaOrm/QormValueCriterion.h>

QormValueCriterion::Ptr QormValueCriterion::equals(const QString &prop, const QVariant &value) {
   return QormValueCriterion::Ptr(new QormValueCriterion(ExpressionType::Equals, prop, value));
}

QString QormValueCriterion::dump() const {
   return QString("%1 %2 %3")
      .arg(prop)
      .arg(expressiontype == ExpressionType::Equals ? " = " : "")
      .arg(value.toString());
}

QString QormValueCriterion::stringify(
   std::function<QString(const QormCriterion *, const QString & /*leftChild*/, const QString &rightChild)> /*containerDelegate*/,
   std::function<QString(const class QormValueCriterion *)> valueDelegate,
   std::function<QString(const class QormListCriterion *)> /*listDelegate*/) const {
   return valueDelegate(this);
}

QormValueCriterion::QormValueCriterion(ExpressionType atype, const QString &aprop, const QVariant &avalue)
   : expressiontype(atype)
   , prop(aprop)
   , value(avalue)
{}
