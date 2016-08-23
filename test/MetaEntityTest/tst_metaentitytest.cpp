#include <QString>
#include <QtTest>

#include <QMetaOrm/metaentity.h>

using namespace QMetaOrm;

class DummyObject : public QObject
{
    Q_OBJECT
public:
    DummyObject() {}
    DummyObject(const DummyObject &) {}
    DummyObject &operator=(const DummyObject &) { return *this; }
    virtual ~DummyObject() {}
};

#define IDPROP "id"

class MetaEntityTest : public QObject
{
    Q_OBJECT

public:
    MetaEntityTest() {}

private Q_SLOTS:

    /**
      * Utils
      */
    MetaEntity metaEntityFromKey(const QString &prop, const QString &field) {
        MetaEntity metaEntity;
        metaEntity.key = qMakePair(prop, field);
        return metaEntity;
    }

    DummyObject objectWithProperty(const QString &name, const QVariant &value) {
        DummyObject o;
        o.setProperty(name.toLocal8Bit(), value);
        return o;
    }

    /**
     * isValid
     */
    void isValid_noKeyAndNoProperties_false() {
        QCOMPARE(MetaEntity().isValid(), false);
    }

    void isValid_noKeyAndAProperty_true() {
        MetaEntity e;
        e.propertyMapping[IDPROP] = IDPROP;
        QCOMPARE(e.isValid(), true);
    }

    void isValid_aKeyAndNoProperty_true() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        QCOMPARE(metaEntity.isValid(), true);
    }

    /**
      * hasValidKey
      */
    void hasValidKey_noKeySpecified_false() {
        QCOMPARE(MetaEntity().hasValidKey(QObject()), false);
    }

    void hasValidKey_objectHasNotAKeyProperty_false() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        QCOMPARE(metaEntity.hasValidKey(QObject()), false);
    }

    void hasValidKey_keyIsOfTypeDouble_false() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, 1.0);
        QCOMPARE(metaEntity.hasValidKey(item), false);
    }

    void hasValidKey_keyIsOfTypeIntWithValue0_false() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, (int)0);
        QCOMPARE(metaEntity.hasValidKey(item), false);
    }

    void hasValidKey_keyIsOfTypeIntWithValue1_true() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, (int)1);
        QCOMPARE(metaEntity.hasValidKey(item), true);
    }

    void hasValidKey_keyIsOfTypeLongLongWithValue0_false() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, (qlonglong)0);
        QCOMPARE(metaEntity.hasValidKey(item), false);
    }

    void hasValidKey_keyIsOfTypeLongLongWithValue1_true() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, (qlonglong)1);
        QCOMPARE(metaEntity.hasValidKey(item), true);
    }

    void hasValidKey_keyIsOfTypeStringWithEmptyString_false() {
        auto metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, QString(""));
        QCOMPARE(metaEntity.hasValidKey(item), false);
    }

    void hasValidKey_keyIsOfTypeStringWithValueNotEmptyString_true() {
        MetaEntity metaEntity = metaEntityFromKey(IDPROP, IDPROP);
        auto item = objectWithProperty(IDPROP, QString("a"));
        QCOMPARE(metaEntity.hasValidKey(item), true);
    }

};

QTEST_APPLESS_MAIN(MetaEntityTest)

#include "tst_metaentitytest.moc"
