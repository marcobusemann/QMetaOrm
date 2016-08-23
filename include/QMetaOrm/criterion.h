#pragma once

#include <QMetaOrm/private.h>

#include <QSharedPointer>
#include <functional>

/**
 * @brief The Criterion class
 */
class QMETAORM_LIBRARY_API Criterion {
public:
   typedef QSharedPointer<Criterion> Ptr;

   enum class CombinationType {
      Leaf,
      And,
      Or
   };

public:
   virtual ~Criterion() {}

   static Criterion::Ptr and(Criterion::Ptr left,  Criterion::Ptr right =  Criterion::Ptr());
   static Criterion::Ptr or(Criterion::Ptr left,  Criterion::Ptr right =  Criterion::Ptr());

   virtual QString dump() const;

   virtual QString stringify(
      std::function<QString(const Criterion *, const QString &leftChild, const QString &rightChild)> containerDelegate,
      std::function<QString(const class ValueCriterion *)> valueDelegate,
      std::function<QString(const class ListCriterion *)> listDelegate) const;

public:
   CombinationType combinationtype;
   Criterion::Ptr left;
   Criterion::Ptr right;

protected:
   Criterion(CombinationType atype, Criterion::Ptr aleft, Criterion::Ptr aright);
   Criterion();
};
