#include <QMetaOrm/entitysqlbuilder.h>
#include <QMetaOrm/valuecriterion.h>
#include <QMetaOrm/listcriterion.h>
#include <QtSql>

using namespace QMetaOrm;

//-----------------------------------------------------------------------------
QString EntitySqlBuilder::buildSelect(MetaEntity mapping) {
   QStringList fields = mapping.propertyMapping.values() << mapping.key.second;
   return QString("SELECT %1 FROM %2 WHERE %3=?")
      .arg(fields.join(","))
      .arg(mapping.source)
      .arg(mapping.key.second);
}

//-----------------------------------------------------------------------------
QString EntitySqlBuilder::buildCriterion(MetaEntity mapping, Criterion::Ptr criterion, QVariantList &conditions) {
   if (!criterion) return "1=1";
   return criterion->stringify([](const Criterion *c, const QString &leftChild, const QString &rightChild) -> QString {
      return QString("((%1) %2 (%3))")
      .arg(leftChild)
      .arg(c->combinationtype == Criterion::CombinationType::Leaf ? "" :
           c->combinationtype == Criterion::CombinationType::And ? " AND " :
           c->combinationtype == Criterion::CombinationType::Or ? " OR " : "")
      .arg(rightChild);
   }, [&conditions](const ValueCriterion *c) -> QString {
      conditions.append(c->value);
      return QString("%1 %2 ?")
               .arg(c->prop)
               .arg(c->expressiontype == ValueCriterion::ExpressionType::Equals ? " = " : "");
   }, [&conditions](const ListCriterion *c) -> QString {
      conditions.append(c->values);
      QStringList params;
      for(int i = 0; i < c->values.size(); i++)
         params << "?";

      return QString(" %1 %2 %3 ")
         .arg(c->prop)
         .arg(c->expressiontype == ListCriterion::ExpressionType::In ? QString(" in (%1)").arg(params.join(",")) : "");
   });
}

// TODO: - Cache build condition
//-----------------------------------------------------------------------------
QString EntitySqlBuilder::buildSelectMany(MetaEntity mapping, Criterion::Ptr criterion, int skip, int pageSize, QVariantList &conditions) {
   QString condition = buildCriterion(mapping, criterion, conditions);
   QStringList fields = mapping.propertyMapping.values() << mapping.key.second;
   return QString("SELECT %1 %2 %3 FROM %4 WHERE %5")
      .arg(pageSize >= 0 ? QString("FIRST %1").arg(pageSize) : "")
      .arg(skip >= 0 ? QString("SKIP %1").arg(skip) : "")
      .arg(fields.join(","))
      .arg(mapping.source)
      .arg(condition);
}

//-----------------------------------------------------------------------------
QString EntitySqlBuilder::buildRemove(MetaEntity mapping) {
   return QString("DELETE FROM %1 WHERE %2=?")
      .arg(mapping.source)
      .arg(mapping.key.first);
}

//-----------------------------------------------------------------------------
QString EntitySqlBuilder::buildInsert(MetaEntity mapping, QStringList &properties) {
   Q_ASSERT_X(!mapping.sequence.isEmpty(), "buildInsert", "actually inserting entities requires a sequence");

   properties = mapping.propertyMapping.keys();

   QStringList fields;
   fields.append(mapping.key.second);
   foreach(auto prop, properties)
      fields.append(mapping.propertyMapping[prop]);

   QStringList params;
   for(int i = 0; i < fields.size() - 1; i++)
      params << "?";

   return QString("INSERT INTO %1 (%2) VALUES (gen_id(%3, 1), %4) RETURNING %5")
      .arg(
         mapping.source,
         fields.join(","),
         mapping.sequence,
         params.join(","),
         mapping.key.second);
}

//-----------------------------------------------------------------------------
QString EntitySqlBuilder::buildUpdate(MetaEntity mapping, QStringList &properties) {
   properties = mapping.propertyMapping.keys();

   QStringList fields;
   foreach(auto prop, properties)
      fields.append(QString("%1=?").arg(mapping.propertyMapping[prop]));

   return QString("UPDATE %1 SET %2 WHERE %3=?")
      .arg(
         mapping.source,
         fields.join(","),
         mapping.key.second);
}
