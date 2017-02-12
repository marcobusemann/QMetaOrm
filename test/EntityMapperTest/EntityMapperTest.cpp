#include <QString>
#include <QtTest>

#include <qsqlfield.h>

#include <QMetaOrm/MetaEntity.h>
#include <QMetaOrm/EntityMapper.h>
#include <QMetaOrm/MetaEntityBuilder.h>

#define QVERIFY_THROW(expression, ExpectedExceptionType) \
do \
{ \
    bool caught_ = false; \
    try { expression; } \
    catch (ExpectedExceptionType const&) { caught_ = true; } \
    catch (...) {} \
    if (!QTest::qVerify(caught_, #expression ", " #ExpectedExceptionType, "", __FILE__, __LINE__))\
        return; \
} while(0)

using namespace QMetaOrm;

#define IDPROPERTY QString("id")
#define PREFIX QString("prefix")

#define DUMMY_SOURCE QString("my_dummy")
#define DUMMY_SEQUENCE QString("my_dummy_gen")
/*
   Simple test class
*/
class DummyObject : public QObject
{
   Q_OBJECT
   Q_PROPERTY(int id READ getId WRITE setId)
   Q_PROPERTY(QString name READ getName WRITE setName)

public:
   struct p {
      static const QString id;
      static const QString name;
   };

   static QSharedPointer<DummyObject> factory() {
      return QSharedPointer<DummyObject>(new DummyObject());
   }

   Q_INVOKABLE DummyObject(): m_id(0) {
   }

   void setId(int id) { m_id = id; }
   int getId() const { return m_id; }

   void setName(const QString &name) { m_name = name; }
   const QString &getName() const { return m_name; }

private:
   int m_id;
   QString m_name;
};

Q_DECLARE_METATYPE(QSharedPointer<DummyObject>)

const QString DummyObject::p::id = "id";
const QString DummyObject::p::name = "id";

namespace QMetaOrm {
   namespace Mappings {
      template <> MetaEntity::Ptr mapping<DummyObject>()
      {
         static const MetaEntity::Ptr map = MetaEntityBuilder::anEntity()
            .forSource(DUMMY_SOURCE)
            .withSequence(DUMMY_SEQUENCE)
            .withId(DummyObject::p::id, "ID")
            .withData(DummyObject::p::name, "NAME")
            .build<DummyObject>();
         return map;
      }
   }
}

class DummyObjectWithoutStdCtor : public QObject
{
Q_OBJECT
Q_PROPERTY(int id READ getId WRITE setId)
public:
   DummyObjectWithoutStdCtor(int) {};

   void setId(int id) { m_id = id; }
   int getId() const { return m_id; }

private:
   int m_id;
};

Q_DECLARE_METATYPE(QSharedPointer<DummyObjectWithoutStdCtor>)


QString NoopPrefixer(const QString &v) {
   return v;
}

class UpperConverter : public QMetaOrm::Converter {
   virtual QVariant convert(const QVariant & value) const override
   {
      return value.type() == QVariant::Type::String ? value.toString().toUpper() : QVariant();
   }
};

class EntityMapperTest : public QObject
{
    Q_OBJECT

public:
   EntityMapperTest() {}

private Q_SLOTS:

    /**
      * Utils
      */
    EntityMapper::Ptr aMapper() {
       return EntityMapper::Ptr(new EntityMapper(StandardQtLogger::factory(), StandardEntityCache::factory()));
    }

    QMetaOrm::MetaEntity::Ptr aDummyMapping() {
       return QMetaOrm::Mappings::mapping<DummyObject>();
    }

   /**
     * Method newObjectFrom
     */
   // TODO: Move to MetaEntity tests
   /*
   void newObjectFrom_stdCtorGiven_validObject() {
      QVERIFY(aMapper().newObjectFrom(aDummyMapping()) != nullptr);
   }

   void newObjectFrom_noStdCtorGiven_null() {
      auto mapping = MetaEntityBuilder::anEntity()
         .forSource(DUMMY_SOURCE)
         .withId("id", "ID")
         .build<DummyObjectWithoutStdCtor>();
      QVERIFY_THROW(aMapper().newObjectFrom(mapping), QMetaOrm::CreatingObjectByMetaObjectException);
   }
   */

   /**
     * Method mapKeyToEntity
     */

   void mapKeyToEntity_mappingExists_handlerCalledWithCorrectPropertyAndValue() {
      QSqlRecord record;
      record.append(QSqlField("ID", QVariant::Type::Int));
      record.setValue("ID", 1);

      auto prefixer = PropertyPrefixer().getRecordValuePrefixer(record);

      bool applied = false;
      aMapper()->mapKeyToEntity(aDummyMapping(), prefixer, [&](const QString &prop, const QVariant &value) {
         applied = true;
         QCOMPARE(prop, QString("id"));
         QCOMPARE(value, QVariant(1));
      });

      if (!applied)
         QFAIL("Did not call apply handler.");
   }

   void mapKeyToEntity_mappingDoesNotExist_handlerNotCalled() {
      QSqlRecord record;
      record.append(QSqlField("ID123", QVariant::Type::Int));
      record.setValue("ID123", 1);

      auto prefixer = PropertyPrefixer().getRecordValuePrefixer(record);

      aMapper()->mapKeyToEntity(aDummyMapping(), prefixer, [&](const QString &, const QVariant &) {
         QFAIL("Mapping should not have been correct!");
      });
   }

   /**
     * Method applyConverter
     */
   void applyConverter_noConverterSpecified_exception() {
      auto store = QMetaOrm::DefaultConverterStore::factory();
      QVERIFY_THROW(aMapper()->applyConverter("A", QVariant(), store), QMetaOrm::ConverterNotFoundException);
   }

   void applyConverter_converterSpecified_valueTransformedCorrectly() {
      auto store = QMetaOrm::DefaultConverterStore::factory();
      store->registerConverter("UpperConverter", QSharedPointer<UpperConverter>(new UpperConverter()));
      QCOMPARE(aMapper()->applyConverter("UpperConverter", QVariant("abc"), store), QVariant("ABC"));
   }


};

QTEST_APPLESS_MAIN(EntityMapperTest)

#include "EntityMapperTest.moc"
