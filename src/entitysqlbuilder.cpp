#include <QMetaOrm/EntitySqlBuilder.h>
#include <QMetaOrm/ValueCriterion.h>
#include <QMetaOrm/ListCriterion.h>
#include <QtSql>

using namespace QMetaOrm;

QString EmbeddAlias(const QString &alias, const QString &field) {
   return QString("%1.%2").arg(alias, field);
}

QStringList EmbeddAlias(const QString &alias, const QStringList &fields) {
   QStringList result;
   for (auto field : fields)
      result.append(QString("%1.%2").arg(alias, field));
   return result;
}

template<class T>
QList<T> map(const QList<T> &container, std::function<T(const T&)> handler) {
   QList<T> result;
   for (auto field : container)
      result.append(handler(field));
   return result;
}

QStringList map(const QStringList &container, std::function<QString(const QString&)> handler) {
   QStringList result;
   for (auto field : container)
      result.append(handler(field));
   return result;
}

class SelectBuilder {
public:
   static SelectBuilder aStatementFor(const QString &table) {
      return SelectBuilder(table);
   }

   SelectBuilder withFields(const QStringList &fields) {
      auto alias = aliasFor(m_table);
      m_fields.append(map(fields, [&](const QString &field) -> QString {
         return QString("%1.%2 AS %2").arg(alias, field);
      }));
      return *this;
   }

   SelectBuilder withCondition(const QString &condition) {
      m_condition = condition;
      return *this;
   }

   SelectBuilder withParameter(const QString &field) {
      auto alias = aliasFor(m_table);
      m_condition = QString("%1=?").arg(EmbeddAlias(alias, field));
      return *this;
   }

   QString ExtractPropertyPrevousPropertyFrom(const QString &prop) {
      int index = prop.lastIndexOf(".");
      return index == -1 ? QString() : prop.left(index);
   }

   SelectBuilder withJoin(const QString &table, const QString &tableField, const QString &referenceTable, const QString &referenceDatabaseField, const QStringList &fields, const QString &fieldPrefix) {
      auto alias = aliasFor(table, fieldPrefix);
      auto aliasRef = aliasFor(referenceTable, ExtractPropertyPrevousPropertyFrom(fieldPrefix));
      m_joins.append(QString("LEFT JOIN %1 %2 ON (%3 = %4)")
         .arg(table)
         .arg(alias)
         .arg(EmbeddAlias(alias, tableField))
         .arg(EmbeddAlias(aliasRef, referenceDatabaseField)));
      m_fields.append(map(fields, [&](const QString &field) -> QString {
         return QString("%1 AS %2_%3")
            .arg(EmbeddAlias(alias, field))
            .arg(QString(fieldPrefix).replace(".", "_"))
            .arg(field);
      }));
      return *this;
   }

   SelectBuilder withCriterion(MetaEntity::Ptr mapping, Criterion::Ptr criterion, QVariantList &conditions) {
      QString condition;
      if (!criterion) 
         condition = "1=1";
      else 
         condition = criterion->stringify([](const Criterion *c, const QString &leftChild, const QString &rightChild) -> QString {
            return QString("((%1) %2 (%3))")
               .arg(leftChild)
               .arg(c->combinationtype == Criterion::CombinationType::Leaf ? "" :
                  c->combinationtype == Criterion::CombinationType::And ? " AND " :
                  c->combinationtype == Criterion::CombinationType::Or ? " OR " : "")
               .arg(rightChild);
         }, [&conditions, &mapping, this](const ValueCriterion *c) -> QString {
            conditions.append(c->value);
            return QString("%1 %2 ?")
               .arg(resolveRecursiveProperty(c->prop.indexOf(".") == -1 ? "" : c->prop, c->prop, mapping))
               .arg(c->expressiontype == ValueCriterion::ExpressionType::Equals ? " = " : "");
         }, [&conditions](const ListCriterion *c) -> QString {
            conditions.append(c->values);
            QStringList params;
            for (int i = 0; i < c->values.size(); i++)
               params << "?";

            return QString(" %1 %2 %3 ")
               .arg(c->prop)
               .arg(c->expressiontype == ListCriterion::ExpressionType::In ? QString(" in (%1)").arg(params.join(",")) : "");
         });
      m_condition = condition;
      return *this;
   }

   SelectBuilder skip(int skipCount) {
      m_skip = skipCount;
      return *this;
   }

   SelectBuilder startWith(int startWithIndex) {
      m_first = startWithIndex;
      return *this;
   }

   QString build() {
      if (m_fields.isEmpty())
         throw std::runtime_error("Building an select statement requires fields."); // TODO: Replace by custom exception

      auto statement = QString("SELECT %1 %2 %3 FROM %4 %5 %6")
         .arg(m_first >= 0 ? QString("FIRST %1").arg(m_first) : "")
         .arg(m_skip >= 0 ? QString("SKIP %1").arg(m_skip) : "")
         .arg(m_fields.join(", "))
         .arg(QString("%1 %2").arg(m_table, aliasFor(m_table)))
         .arg(m_joins.join(" "))
         .arg(m_condition.isEmpty() ? "" : QString("WHERE %1").arg(m_condition))
         ;
      return statement;
   }

private:
   SelectBuilder(const QString &table)
      : m_nextAliasNumber(0)
      , m_table(table)
      , m_first(-1)
      , m_skip(-1)
   {}

   QString aliasFor(const QString &table, const QString &prop = QString()) {
      auto value = qMakePair(table, prop);
      if (!m_aliasCache.contains(value))
         m_aliasCache[value] = QString("a%1").arg(m_nextAliasNumber++);
      return m_aliasCache[value];
   }

   QString resolveRecursiveProperty(const QString &rawProperty, const QString &prop, MetaEntity::Ptr entity) {
      int index = prop.indexOf(".");
      if (index == -1)
      {
         QString alias = aliasFor(entity->getSource(), rawProperty);
         QString databaseName = 
            prop == entity->getKeyProperty() ? entity->getKeyDatabaseField() :
            entity->getProperty(prop).databaseName;
         return EmbeddAlias(alias, databaseName);
      }
      QString referenceProperty = prop.left(index);
      auto referencePropertyEntity = entity->getProperty(referenceProperty);
      if (!referencePropertyEntity.isReference()) {
         qDebug() << "No reference! " << prop;
         return prop;
      }
      return resolveRecursiveProperty(rawProperty, prop.mid(index + 1), referencePropertyEntity.reference);
   }

   QHash<QPair<QString, QString>, QString> m_aliasCache; // Key = Table-Name, Value = Alias
   int m_nextAliasNumber;
   QStringList m_fields;
   QString m_table;
   QString m_condition;
   QStringList m_joins;
   int m_first;
   int m_skip;
};

void ConstructSelectRecursive(SelectBuilder &builder, MetaEntity::Ptr mapping, const QString &prefix = QString()) {
   auto references = mapping->getReferences();
   for (int i = 0; i < references.size(); i++) {
      auto refType = references[i].reference;
      auto refPrefix = prefix.isEmpty() ? 
         references[i].databaseName : 
         QString("%1.%2").arg(prefix, references[i].databaseName);

      builder
         .withJoin(
            refType->getSource(),
            refType->getKeyDatabaseField(),
            mapping->getSource(),
            references[i].databaseName,
            refType->getDatabaseFields(),
            refPrefix);

      ConstructSelectRecursive(builder, refType, refPrefix);
   }
}

SelectBuilder ConstructSelect(MetaEntity::Ptr mapping) {
   auto builder = SelectBuilder::aStatementFor(mapping->getSource())
      .withFields(mapping->getDatabaseFields());
   ConstructSelectRecursive(builder, mapping);
   return builder;
}

QString EntitySqlBuilder::buildSelect(MetaEntity::Ptr mapping) {
   
   return ConstructSelect(mapping)
      .withParameter(mapping->getKeyDatabaseField())
      .build();
}

// TODO: - Cache build condition
QString EntitySqlBuilder::buildSelectMany(MetaEntity::Ptr mapping, Criterion::Ptr criterion, int skip, int pageSize, QVariantList &conditions) {
   return ConstructSelect(mapping)
      .startWith(pageSize)
      .skip(skip)
      .withCriterion(mapping, criterion, conditions)
      .build();
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
