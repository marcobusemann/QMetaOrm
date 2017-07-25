#include <QMetaOrm/QormMetaEntity.h>

#include <qstring.h>
#include <qsharedpointer.h>
#include <qobject.h>

class PersonSimple : public QObject {
Q_OBJECT

    Q_PROPERTY(int id
        READ
            getId
        WRITE
        setId)
    Q_PROPERTY(QString surname
        READ
            getSurname
        WRITE
        setSurname)
    Q_PROPERTY(QString name
        READ
            getName
        WRITE
        setName)

public:
    typedef QSharedPointer<PersonSimple> Ptr;

public:
    struct p {
        static const QString id;
        static const QString name;
        static const QString surname;
    };

public:
    PersonSimple()
        :m_id(0)
    {
    }

    bool operator==(const PersonSimple& aRhs)
    {
        return
            m_id==aRhs.m_id &&
                m_name==aRhs.m_name &&
                m_surname==aRhs.m_surname;
    }

    int getId() const { return m_id; }

    void setId(int aId) { m_id = aId; }

    const QString& getSurname() const { return m_surname; }

    const QString& getName() const { return m_name; }

    void setSurname(const QString& aSurname) { m_surname = aSurname; }

    void setName(const QString& aName) { m_name = aName; }

private:
    int m_id;
    QString m_surname;
    QString m_name;
};

Q_DECLARE_METATYPE(PersonSimple::Ptr)

class Address : public QObject {
Q_OBJECT

    Q_PROPERTY(int id
        READ
            getId
        WRITE
        setId)
    Q_PROPERTY(QString country
        READ
            getCountry
        WRITE
        setCountry)
    Q_PROPERTY(QString postCode
        READ
            getPostCode
        WRITE
        setPostCode)
    Q_PROPERTY(QString street
        READ
            getStreet
        WRITE
        setStreet)

public:
    typedef QSharedPointer<Address> Ptr;

public:
    struct p {
        static const QString id;
        static const QString country;
        static const QString postCode;
        static const QString street;
    };

    Address()
        :
        m_id(0)
    {
    }

    int getId() const { return m_id; }

    void setId(int aId) { m_id = aId; }

    QString getCountry() const { return m_country; }

    void setCountry(const QString& aCountry) { m_country = aCountry; }

    QString getPostCode() const { return m_postCode; }

    void setPostCode(const QString& aPostCode) { m_postCode = aPostCode; }

    QString getStreet() const { return m_street; }

    void setStreet(const QString& aStreet) { m_street = aStreet; }

private:
    int m_id;
    QString m_country;
    QString m_postCode;
    QString m_street;
};

Q_DECLARE_METATYPE(Address::Ptr);

class PersonComplex : public QObject {
Q_OBJECT

    Q_PROPERTY(int id
        READ
            getId
        WRITE
        setId)
    Q_PROPERTY(QString surname
        READ
            getSurname
        WRITE
        setSurname)
    Q_PROPERTY(QString name
        READ
            getName
        WRITE
        setName)
    Q_PROPERTY(Address::Ptr address
        READ
            getAddress
        WRITE
        setAddress)

public:
    typedef QSharedPointer<PersonComplex> Ptr;

public:
    struct p {
        static const QString id;
        static const QString name;
        static const QString surname;
        static const QString address;
    };

public:
    PersonComplex()
        :m_id(0)
    {
    }

    bool operator==(const PersonComplex& aRhs)
    {
        return
            m_id==aRhs.m_id &&
                m_name==aRhs.m_name &&
                m_surname==aRhs.m_surname;
    }

    int getId() const { return m_id; }

    void setId(int aId) { m_id = aId; }

    const QString& getSurname() const { return m_surname; }

    const QString& getName() const { return m_name; }

    void setSurname(const QString& aSurname) { m_surname = aSurname; }

    void setName(const QString& aName) { m_name = aName; }

    const Address::Ptr& getAddress() const { return m_address; }

    void setAddress(const Address::Ptr& aAddress) { m_address = aAddress; }

private:
    int m_id;
    QString m_surname;
    QString m_name;
    Address::Ptr m_address;
};

Q_DECLARE_METATYPE(PersonComplex::Ptr)

namespace QormMappings {
    QormMetaEntity::Ptr TsPersonSimpleMapping();

    QormMetaEntity::Ptr TsPersonComplexMapping();

    QormMetaEntity::Ptr TsAddressMapping();
}
