#pragma once

#include <QMetaOrm/MetaEntity.h>
#include <QMetaOrm/Criterion.h>
#include <QMetaOrm/ValueCriterion.h>
#include <QMetaOrm/ListCriterion.h>
#include <QMetaOrm/SessionFactory.h>
#include <QMetaOrm\MetaEntityBuilder.h>

#include <QObject>
#include <QDateTime>
#include <QSharedPointer>
#include <QDebug>

class Address : public QObject
{
   Q_OBJECT

      Q_PROPERTY(int id           READ getId      WRITE setId)
      Q_PROPERTY(QString country  READ getCountry WRITE setCountry)
      Q_PROPERTY(QString street   READ getStreet  WRITE setStreet)
      Q_PROPERTY(QString plz      READ getPlz     WRITE setPlz)

public:
   struct p {
      static const QString id;
      static const QString country;
      static const QString street;
      static const QString plz;
   };

   typedef QSharedPointer<Address> Ptr;

public:
   Address() : m_id(0) {}
   Address(const Address &rhs)
      : m_id(rhs.m_id)
      , m_country(rhs.m_country)
      , m_street(rhs.m_street)
      , m_plz(rhs.m_plz)
   {
   }

   Address &operator=(const Address &rhs) {
      m_id = rhs.m_id;
      m_country = rhs.m_country;
      m_street = rhs.m_street;
      m_plz = rhs.m_plz;
      return *this;
   }

   int getId() const { return m_id; }
   void setId(int id) { m_id = id; }

   const QString &getCountry() const { return m_country; }
   void setCountry(const QString &country) { m_country = country; }

   const QString &getStreet() const { return m_street; }
   void setStreet(const QString &street) { m_street = street; }

   const QString &getPlz() const { return m_plz; }
   void setPlz(const QString &plz) { m_plz = plz; }

   void dump() {
      qDebug() << "Address: " << m_id << m_country << m_street << m_plz;
   }

private:
   int m_id;
   QString m_country;
   QString m_street;
   QString m_plz;


};

Q_DECLARE_METATYPE(Address);
Q_DECLARE_METATYPE(Address::Ptr);

namespace QMetaOrm {
   namespace Mappings {
      template <> QMetaOrm::MetaEntity::Ptr mapping<Address>();
   }
}

class PersonSimple : public QObject
{
   Q_OBJECT

      Q_PROPERTY(int id               READ getId        WRITE setId)
      Q_PROPERTY(QString surname      READ getSurname   WRITE setSurname)
      Q_PROPERTY(QString lastname     READ getLastname  WRITE setLastname)
      Q_PROPERTY(QDateTime birthdate  READ getBirthdate WRITE setBirthdate)
      Q_PROPERTY(Address::Ptr address READ getAddress   WRITE setAddress)

public:
   struct p {
      static const QString id;
      static const QString surname;
      static const QString lastname;
      static const QString birthdate;
      static const QString address;
   };

   typedef QSharedPointer<PersonSimple> Ptr;

public:
   PersonSimple() : m_id(0) {}
   PersonSimple(const PersonSimple &rhs)
      : m_id(rhs.m_id)
      , m_surname(rhs.m_surname)
      , m_lastname(rhs.m_lastname)
      , m_birthdate(rhs.m_birthdate)
      , m_address(rhs.m_address)
   {
   }

   PersonSimple &operator=(const PersonSimple &rhs) {
      m_id = rhs.m_id;
      m_surname = rhs.m_surname;
      m_lastname = rhs.m_lastname;
      m_birthdate = rhs.m_birthdate;
      m_address = rhs.m_address;
      return *this;
   }

   int getId() const { return m_id; }
   void setId(int id) { m_id = id; }

   const QString &getSurname() const { return m_surname; }
   void setSurname(const QString &surname) { m_surname = surname; }

   const QString &getLastname() const { return m_lastname; }
   void setLastname(const QString &lastname) { m_lastname = lastname; }

   const QDateTime &getBirthdate() const { return m_birthdate; }
   void setBirthdate(const QDateTime &birthdate) { m_birthdate = birthdate; }

   const Address::Ptr &getAddress() const { return m_address; }
   void setAddress(const Address::Ptr &address) { m_address = address; }

   void dump() {
      qDebug() << "Person: " << m_id << m_surname << m_lastname << m_birthdate;
      if (m_address != nullptr)
         m_address->dump();
   }

private:
   int m_id;
   QString m_surname;
   QString m_lastname;
   QDateTime m_birthdate;
   Address::Ptr m_address;

signals:
   void propertyChanged();
};

Q_DECLARE_METATYPE(PersonSimple);
Q_DECLARE_METATYPE(PersonSimple::Ptr);

namespace QMetaOrm {
   namespace Mappings {
      template <> QMetaOrm::MetaEntity::Ptr mapping<PersonSimple>();
   }
}