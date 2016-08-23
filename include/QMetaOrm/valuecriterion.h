#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/criterion.h>

#include <QVariant>

namespace QMetaOrm {

   /**
    * @brief The ValueCriterion class
    */
   class QMETAORM_LIBRARY_API ValueCriterion: public Criterion {
   public:
      typedef QSharedPointer<ValueCriterion> Ptr;

      enum class ExpressionType {
         Equals
      };

   public:
      static ValueCriterion::Ptr equals(const QString &prop, const QVariant &value);

      virtual QString dump() const override;
      virtual QString stringify(
         std::function<QString(const Criterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
         std::function<QString(const class ValueCriterion *)> valueDelegate,
         std::function<QString(const class ListCriterion *)> listDelegate) const override;

   public:
      ExpressionType expressiontype;
      QString prop;
      QVariant value;

   private:
      ValueCriterion(ExpressionType atype, const QString &aprop, const QVariant &avalue);
   };
}