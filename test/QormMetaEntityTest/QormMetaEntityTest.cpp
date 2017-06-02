#include <QString>
#include <QtTest>

#include <QMetaOrm/QormMetaEntity.h>

class DummyObject : public QObject
{
   Q_OBJECT
};

#define IDPROP "id"

class QormMetaEntityTest : public QObject
{
   Q_OBJECT

public:
   QormMetaEntityTest() {}

private slots :

   /**
     * Utils
     */
   QormMetaEntity::Ptr metaEntityFromKey(const QString &prop, const QString &field) {
      auto metaEntity = QormMetaEntity::factory();
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
      QCOMPARE(QormMetaEntity::factory()->hasValidKey(QSharedPointer<QObject>()), false);
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

QTEST_APPLESS_MAIN(QormMetaEntityTest)

#include "QormMetaEntityTest.moc"
