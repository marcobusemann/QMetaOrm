#pragma once

#include <QMetaOrm/metaentity.h>
#include <QMetaOrm/criterion.h>
#include <QMetaOrm/valuecriterion.h>
#include <QMetaOrm/listcriterion.h>
#include <QMetaOrm/sessionfactory.h>

#include <QObject>
#include <QDateTime>
#include <QSharedPointer>
#include <QDebug>

class Todo: public QObject
{
   Q_OBJECT

   Q_PROPERTY(int id          READ getId     WRITE setId    NOTIFY propertyChanged)
   Q_PROPERTY(QString title   READ getTitle  WRITE setTitle NOTIFY propertyChanged)
   Q_PROPERTY(QDateTime start READ getStart  WRITE setStart NOTIFY propertyChanged)
   Q_PROPERTY(QDateTime end   READ getEnd    WRITE setEnd   NOTIFY propertyChanged)

public:
   struct p {
      static const QString id;
      static const QString title;
      static const QString start;
      static const QString end;
   };

public:
   Todo(): m_id(0) {}
   Todo(const Todo &rhs)
      : m_id(rhs.m_id)
      , m_title(rhs.m_title)
      , m_start(rhs.m_start)
      , m_end(rhs.m_end)
   {
   }

   Todo &operator=(const Todo &rhs) {
      m_id = rhs.m_id;
      m_title = rhs.m_title;
      m_start = rhs.m_start;
      m_end = rhs.m_end;
      return *this;
   }

   virtual ~Todo() {}

   int getId() const { return m_id; }
   void setId(int id) { m_id = id; }

   const QString &getTitle() const { return m_title; }
   void setTitle(const QString &title) {
      if (m_title == title) return;
      m_title = title;
      emit propertyChanged();
   }

   const QDateTime &getStart() const { return m_start; }
   void setStart(const QDateTime &start) {
      if (m_start == start) return;
      m_start = start;
      emit propertyChanged();
   }

   const QDateTime &getEnd() const { return m_end; }
   void setEnd(const QDateTime &end) {
      if (m_end == end) return;
      m_end = end;
      emit propertyChanged();
   }

   void dump() {
      qDebug() << "Todo: " << m_id << m_title << m_start << m_end;
   }

private:
   int m_id;
   QString m_title;
   QDateTime m_start;
   QDateTime m_end;

signals:
   void propertyChanged();
};

namespace QMetaOrm {
   namespace Mappings {
      template <> MetaEntity::Ptr mapping<Todo>();
   }
}

class TodoBuilder
{
public:
   static TodoBuilder aTodo() {
      return TodoBuilder();
   }

   TodoBuilder withId(int id) {
      m_entity.setId(id);
      return *this;
   }

   TodoBuilder withTitle(const QString &title) {
      m_entity.setTitle(title);
      return *this;
   }

   TodoBuilder withStart(const QDateTime &start) {
      m_entity.setStart(start);
      return *this;
   }

   TodoBuilder withEnd(const QDateTime &end) {
      m_entity.setEnd(end);
      return *this;
   }

   TodoBuilder withInterval(const QDateTime &start, const QDateTime &end) {
      m_entity.setStart(start);
      m_entity.setEnd(end);
      return *this;
   }

   Todo build() {
      return m_entity;
   }

private:
   TodoBuilder() {}
   Todo m_entity;
};

class TodoCriterionData : public QSharedData
{
public:
   TodoCriterionData() { }
   TodoCriterionData(const TodoCriterionData &other)
      : QSharedData(other), m_parent(other.m_parent), m_combination(other.m_combination) { }
   ~TodoCriterionData() { }

   QSharedDataPointer<TodoCriterionData> m_parent;
   QMetaOrm::Criterion::Ptr m_combination;
};

class TodoCriterionBuilder
{
public:

   TodoCriterionBuilder(const TodoCriterionBuilder &other)
         : m_data (other.m_data)
   {}

   static TodoCriterionBuilder aTodoWhere() {
      return TodoCriterionBuilder();
   }

   TodoCriterionBuilder idEquals(int id) {
      m_data->m_combination = QMetaOrm::Criterion::and(m_data->m_combination, QMetaOrm::ValueCriterion::equals(Todo::p::id, id));
      return *this;
   }

   TodoCriterionBuilder titleEquals(const QString &title) {
      m_data->m_combination = QMetaOrm::Criterion::and(m_data->m_combination, QMetaOrm::ValueCriterion::equals(Todo::p::title, title));
      return *this;
   }

   TodoCriterionBuilder startEquals(const QDateTime &start) {
      m_data->m_combination = QMetaOrm::Criterion::and(m_data->m_combination, QMetaOrm::ValueCriterion::equals(Todo::p::start, start));
      return *this;
   }

   TodoCriterionBuilder endEquals(const QString &end) {
      m_data->m_combination = QMetaOrm::Criterion::and(m_data->m_combination, QMetaOrm::ValueCriterion::equals(Todo::p::end, end));
      return *this;
   }

   TodoCriterionBuilder or(TodoCriterionBuilder &criterium) {
      m_data->m_combination = QMetaOrm::Criterion::or(m_data->m_combination, criterium.m_data->m_combination);
      return *this;
   }

   TodoCriterionBuilder or() {
      TodoCriterionBuilder another;
      another.m_data->m_parent = m_data;
      return another;
   }

   TodoCriterionBuilder end() {
      if (!m_data->m_parent) return *this;
      TodoCriterionBuilder end(m_data->m_parent);
      end.m_data->m_combination = QMetaOrm::Criterion::or(end.m_data->m_combination, m_data->m_combination);
      return end;
   }

   QMetaOrm::Criterion::Ptr build() {
      return m_data->m_combination;
   }

private:
   TodoCriterionBuilder() { m_data = new TodoCriterionData; }
   TodoCriterionBuilder(QSharedDataPointer<TodoCriterionData> data): m_data(data) {}

   QSharedDataPointer<TodoCriterionData> m_data;
};

//-----------------------------------------------------------------------------
template <class T, typename Key>
class IRepository
{
public:
   virtual ~IRepository() {}

   virtual T save(T item) = 0;
   virtual void remove(T item) = 0;

   virtual T get(Key id) const = 0;
   virtual QList<T> find(QMetaOrm::Criterion::Ptr criterion, int skip = -1, int pageSize = -1) = 0;
};

//-----------------------------------------------------------------------------
class TodoRepository: public IRepository<Todo, int>
{
public:
   TodoRepository(QMetaOrm::SessionFactory::Ptr sessionFactory)
      : m_sessionFactory(sessionFactory)
   {
   }

   virtual ~TodoRepository() {}

   virtual Todo save(Todo item) {
      auto session = m_sessionFactory->createSession();
      Todo result = session->save(item);
      session->commit();
      return result;
   }

   virtual void remove(Todo item) {
      auto session = m_sessionFactory->createSession();
      session->remove(item);
      session->commit();
   }

   virtual Todo get(int id) const {
      auto session = m_sessionFactory->createSession();
      return session->selectOne<Todo>(id);
   }

   virtual QList<Todo> find(QMetaOrm::Criterion::Ptr criterion, int skip = -1, int pageSize = -1) {
      auto session = m_sessionFactory->createSession();
      return session->selectMany<Todo>(criterion, skip, pageSize);
   }

private:
   QMetaOrm::SessionFactory::Ptr m_sessionFactory;
};

//-----------------------------------------------------------------------------
class ICommand {
public:
   virtual ~ICommand() {}
   virtual void execute() = 0;
};

//-----------------------------------------------------------------------------
template <class T>
class IQuery {
public:
   virtual ~IQuery() {}
   virtual QList<T> query();
};