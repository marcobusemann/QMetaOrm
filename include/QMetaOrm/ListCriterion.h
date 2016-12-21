#pragma once

#include <QMetaOrm/Private.h>
#include <QMetaOrm/Criterion.h>

#include <QSharedPointer>
#include <QVariantList>
#include <QStringList>

namespace QMetaOrm {

   /**
     *
     */
   class QMETAORM_LIBRARY_API ListCriterion: public Criterion {
   public:
      typedef QSharedPointer<ListCriterion> Ptr;

      enum class ExpressionType {
         In
      };

   public:
      static ListCriterion::Ptr in(const QString &prop, const QVariantList &values);

      virtual QString dump() const override;

      virtual QString stringify(
         std::function<QString(const Criterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
         std::function<QString(const class ValueCriterion *)> valueDelegate,
         std::function<QString(const class ListCriterion *)> listDelegate) const override;

   public:
      ExpressionType expressiontype;
      QString prop;
      QList<QVariant> values;

   private:
      ListCriterion(ExpressionType atype, const QString &aprop, const QVariantList &avalues);
   };
}