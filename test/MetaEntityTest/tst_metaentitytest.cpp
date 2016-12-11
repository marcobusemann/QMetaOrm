#include <QString>
#include <QtTest>

#include <QMetaOrm/metaentity.h>

using namespace QMetaOrm;

class DummyObject : public QObject
{
   Q_OBJECT
};

#define IDPROP "id"

class MetaEntityTest : public QObject
{
   Q_OBJECT

public:
   MetaEntityTest() {}

   private Q_SLOTS :

      /**
        * Utils
        */
      MetaEntity::Ptr metaEntityFromKey(const QString &prop, const QString &field) {
      auto metaEntity = MetaEntity::factory();
      metaEntity->setKey(prop, field);
      return metaEntity;
   }

   QSharedPointer<DummyObject> objectWithProperty(const QString &name, const QVariant &value) {
      QSharedPointer<DummyObject> o(new DummyObject());
      o->setProperty(name.toLocal8Bit(), value);
      return o;
   }

   /**
     * hasValidKey
     */
   void hasValidKey_noKeySpecified_false() {
      QCOMPARE(MetaEntity::factory()->hasValidKey(QSharedPointer<QObject>()), false);
   }

   void hasValidKey_objectHasNotAKeyProperty_false() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      QCOMPARE(metaEntity->hasValidKey(QSharedPointer<QObject>()), false);
   }

   void hasValidKey_keyIsOfTypeDouble_false() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, 1.0);
      QCOMPARE(metaEntity->hasValidKey(item), false);
   }

   void hasValidKey_keyIsOfTypeIntWithValue0_false() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, (int)0);
      QCOMPARE(metaEntity->hasValidKey(item), false);
   }

   void hasValidKey_keyIsOfTypeIntWithValue1_true() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, (int)1);
      QCOMPARE(metaEntity->hasValidKey(item), true);
   }

   void hasValidKey_keyIsOfTypeLongLongWithValue0_false() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, (qlonglong)0);
      QCOMPARE(metaEntity->hasValidKey(item), false);
   }

   void hasValidKey_keyIsOfTypeLongLongWithValue1_true() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, (qlonglong)1);
      QCOMPARE(metaEntity->hasValidKey(item), true);
   }

   void hasValidKey_keyIsOfTypeStringWithEmptyString_false() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, QString(""));
      QCOMPARE(metaEntity->hasValidKey(item), false);
   }

   void hasValidKey_keyIsOfTypeStringWithValueNotEmptyString_true() {
      auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
      auto item = objectWithProperty(IDPROP, QString("a"));
      QCOMPARE(metaEntity->hasValidKey(item), true);
   }

};

QTEST_APPLESS_MAIN(MetaEntityTest)

#include "tst_metaentitytest.moc"
