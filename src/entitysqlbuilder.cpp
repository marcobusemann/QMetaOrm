#include <QMetaOrm/entitysqlbuilder.h>
#include <QMetaOrm/valuecriterion.h>
#include <QMetaOrm/listcriterion.h>
#include <QtSql>

using namespace QMetaOrm;

QString EntitySqlBuilder::buildSelect(MetaEntity::Ptr mapping) {
   QStringList fields = mapping->getDatabaseFields();
   return QString("SELECT %1 FROM %2 WHERE %3=?")
      .arg(fields.join(","))
      .arg(mapping->getSource())
      .arg(mapping->getKeyDatabaseField());
}

QString EntitySqlBuilder::buildCriterion(MetaEntity::Ptr mapping, Criterion::Ptr criterion, QVariantList &conditions) {
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
QString EntitySqlBuilder::buildSelectMany(MetaEntity::Ptr mapping, Criterion::Ptr criterion, int skip, int pageSize, QVariantList &conditions) {
   QString condition = buildCriterion(mapping, criterion, conditions);
   QStringList fields = mapping->getDatabaseFields();
   return QString("SELECT %1 %2 %3 FROM %4 WHERE %5")
      .arg(pageSize >= 0 ? QString("FIRST %1").arg(pageSize) : "")
      .arg(skip >= 0 ? QString("SKIP %1").arg(skip) : "")
      .arg(fields.join(","))
      .arg(mapping->getSource())
      .arg(condition);
}

QString EntitySqlBuilder::buildRemove(MetaEntity::Ptr mapping) {
   return QString("DELETE FROM %1 WHERE %2=?")
      .arg(mapping->getSource())
      .arg(mapping->getKeyProperty());
}

QString EntitySqlBuilder::buildInsert(MetaEntity::Ptr mapping, QStringList &properties) {
   Q_ASSERT_X(mapping->hasSequence(), "buildInsert", "actually inserting entities requires a sequence");

   properties = mapping->getProperties();

   QStringList fields;
   fields.append(mapping->getKeyDatabaseField());
   foreach(auto prop, properties)
      fields.append(mapping->getProperty(prop).databaseName);

   QStringList params;
   for(int i = 0; i < fields.size() - 1; i++)
      params << "?";

   return QString("INSERT INTO %1 (%2) VALUES (gen_id(%3, 1), %4) RETURNING %5")
      .arg(
         mapping->getSource(),
         fields.join(","),
         mapping->getSequence(),
         params.join(","),
         mapping->getKeyDatabaseField());
}

QString EntitySqlBuilder::buildUpdate(MetaEntity::Ptr mapping, QStringList &properties) {
   properties = mapping->getProperties();

   QStringList fields;
   foreach(auto prop, properties)
      fields.append(QString("%1=?").arg(mapping->getProperty(prop).databaseName));

   return QString("UPDATE %1 SET %2 WHERE %3=?")
      .arg(
         mapping->getSource(),
         fields.join(","),
         mapping->getKeyDatabaseField());
}
