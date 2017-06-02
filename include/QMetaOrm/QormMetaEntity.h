#pragma once

#include <QMetaOrm/QormConverterStore.h>
#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QHash>
#include <QPair>
#include <QList>

#include <functional>

// General utility: if_<Condition, Then, Else>::type
// Selects 'Then' or 'Else' type based on the value of 
// the 'Condition'
template <bool Condition, typename Then, typename Else = void>
struct if_ {
   typedef Then type;
};

template <typename Then, typename Else>
struct if_<false, Then, Else > {
   typedef Else type;
};

class QormEntityFactory {
public:
   typedef QSharedPointer<QormEntityFactory> Ptr;

public:
   virtual ~QormEntityFactory() {}
   virtual QSharedPointer<QObject> construct() const = 0;
   virtual QVariant pack(const QSharedPointer<QObject> &entity) const = 0;
   virtual QSharedPointer<QObject> unpack(const QVariant &value) const = 0;
};

template <class T, typename _ = void>
class QormDefaultEntityFactory : public QormEntityFactory {
public:
   QormDefaultEntityFactory() {
      qRegisterMetaType<QSharedPointer<T>>();
   }
   virtual QSharedPointer<QObject> construct() const override {
      return QSharedPointer<T>(new T());
   }

   virtual QVariant pack(const QSharedPointer<QObject> &entity) const override {
      return QVariant::fromValue(static_cast<QSharedPointer<T>>(entity.objectCast<T>()));
   }

   virtual QSharedPointer<QObject> unpack(const QVariant &packetValue) const override {
      return *reinterpret_cast<const QSharedPointer<T> *>(packetValue.constData());
   }
};

template <class T>
class QormDefaultEntityFactory<T, typename if_<false, typename T::Ptr>::type> : public QormEntityFactory {
public:
   QormDefaultEntityFactory() {
      qRegisterMetaType<typename T::Ptr>();
   }

   virtual QSharedPointer<QObject> construct() const override {
      return typename T::Ptr(new T());
   }

   virtual QVariant pack(const QSharedPointer<QObject> &entity) const override {
      return QVariant::fromValue(static_cast<typename T::Ptr>(entity.objectCast<T>()));
   }

   virtual QSharedPointer<QObject> unpack(const QVariant &packetValue) const override {
      return *reinterpret_cast<const typename T::Ptr *>(packetValue.constData());
   }
};

struct QMETAORM_LIBRARY_API QormMetaProperty
{
   QString propertyName;
   QString databaseName;
   QString converterName;
   QSharedPointer<class QormMetaEntity> reference;

   bool hasConverter() const {
      return !converterName.isEmpty();
   }

   bool isReference() {
      return reference != nullptr;
   }
};

enum class KeyGenerationStrategy {
   Sequence,
   Identity
};

/**
* @brief The MetaEntity struct
*/
class QMETAORM_LIBRARY_API QormMetaEntity
{
public:
   typedef std::function<QVariant(const QSharedPointer<QObject> &)> ReferenceCaster;
   typedef std::function<QSharedPointer<QObject>(const QVariant &)> VariantToReferenceCaster;
   typedef std::function<QObject*()> ObjectConstructor;
   typedef QSharedPointer<QormMetaEntity> Ptr;
   static Ptr factory() {
      return Ptr(new QormMetaEntity());
   }

public:
   static const QList<QVariant::Type> SupportedKeyTypes;

   QormMetaEntity(const QormMetaEntity &rhs);

   void setSource(const QString &aSource);
   const QString &getSource() const;
   bool hasSource() const;

   void setSequence(const QString &aSequence);
   const QString &getSequence() const;
   bool hasSequence() const;
   KeyGenerationStrategy getKeyGenerationStrategy() const;

   void setKey(const QString &aProperty, const QString &aDatabaseField);
   const QString &getKeyProperty() const;
   const QString &getKeyDatabaseField() const;
   bool hasKey() const;

   QList<QString> getProperties() const;
   const QormMetaProperty &getProperty(const QString &aProperty);
   void addProperty(const QormMetaProperty &prop);

   QList<QormMetaProperty> getReferences() const;

   QormEntityFactory::Ptr getEntityFactory() const;
   void setEntityFactory(const QormEntityFactory::Ptr &entityFactory);

   template <class T>
   bool hasValidKey(const QSharedPointer<T> &item) const {
      auto keyValue = getProperty(item, getKeyProperty());
      if (!keyValue.isValid()) return false;
      return
         keyValue.type() == QVariant::Int ? keyValue.toInt() > 0 :
      keyValue.type() == QVariant::LongLong ? keyValue.toLongLong() > 0 :
      keyValue.type() == QVariant::String ? !keyValue.toString().isEmpty() : false;
   }

   QStringList getDatabaseFields() const;

   QVariant getProperty(const QSharedPointer<QObject> &item, const QString &name) const {
      return item == nullptr ? QVariant() : item->property(name.toStdString().c_str());
   }

   template <class T>
   QVariant getFlatPropertyValue(const QSharedPointer<T> &item, const QString &prop, QormConverterStore::Ptr converterStore) const {
      auto propMeta = m_propertyMapping[prop];
      QVariant result;
      if (propMeta.isReference())
      {
         auto referencedItem = m_entityFactory->unpack(getProperty(item, prop));
         auto keyProperty = propMeta.reference->getKeyProperty();
         result = getProperty(referencedItem, keyProperty);
      }
      else
         result = getProperty(item, prop);

      if (propMeta.hasConverter()) {
         if (!converterStore->hasConverter(propMeta.converterName))
            qDebug() << "Converter " << propMeta.converterName << " not found!";
         else
            result = converterStore->getConverterFor(propMeta.converterName)->convert(result);
      }

      return result;
   }

   template <class T>
   void setProperty(QSharedPointer<T> &item, const QString &name, const QVariant &value) const {
      item->setProperty(name.toStdString().c_str(), value);
   }

   template <class T>
   void setProperty(T *item, const QString &name, const QVariant &value) const {
      item->setProperty(name.toStdString().c_str(), value);
   }

   Ptr copy();

private:
   friend class QormMetaEntityBuilder;
   QormMetaEntity();

   QString m_source;
   QPair<QString, QString> m_key;
   QHash<QString, QormMetaProperty> m_propertyMapping;

   KeyGenerationStrategy m_keyGenerationstrategy;
   QString m_sequence;

   QormEntityFactory::Ptr m_entityFactory;
};

namespace QormMappings {
   template <class T> QormMetaEntity::Ptr mapping() { return QormMetaEntity::Ptr(); }
}

Q_DECLARE_METATYPE(QSharedPointer<QObject>)
