#pragma once

#include <QMetaOrm/Private.h>
#include <QMetaOrm/ConverterStore.h>

#include <qsharedpointer.h>
#include <qstringlist.h>
#include <qvariant.h>
#include <qhash.h>
#include <qpair.h>
#include <qlist.h>

#include <functional>

#include <qdebug.h>

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

namespace QMetaOrm {

   class EntityFactory {
   public:
      typedef QSharedPointer<EntityFactory> Ptr;

   public:
      virtual ~EntityFactory() {}
      virtual QSharedPointer<QObject> construct() const = 0;
      virtual QVariant pack(const QSharedPointer<QObject> &entity) const = 0;
      virtual QSharedPointer<QObject> unpack(const QVariant &value) const = 0;
   };

   template <class T, typename _ = void>
   class DefaultEntityFactory : public EntityFactory {
   public:
      DefaultEntityFactory() {
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
   class DefaultEntityFactory<T, typename if_<false, typename T::Ptr>::type> : public EntityFactory {
   public:
      DefaultEntityFactory() {
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

   /**
   * @brief The MetaProperty struct
   */
   struct QMETAORM_LIBRARY_API MetaProperty
   {
      QString propertyName;
      QString databaseName;
      QString converterName;
      QSharedPointer<class MetaEntity> reference;

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
   class QMETAORM_LIBRARY_API MetaEntity
   {
   public:
      typedef std::function<QVariant(const QSharedPointer<QObject> &)> ReferenceCaster;
      typedef std::function<QSharedPointer<QObject>(const QVariant &)> VariantToReferenceCaster;
      typedef std::function<QObject*()> ObjectConstructor;
      typedef QSharedPointer<MetaEntity> Ptr;
      static Ptr factory() {
         return Ptr(new MetaEntity());
      }

   public:
      static const QList<QVariant::Type> SupportedKeyTypes;

      MetaEntity(const MetaEntity &rhs);

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
      const MetaProperty &getProperty(const QString &aProperty);
      void addProperty(const MetaProperty &prop);

      QList<MetaProperty> getReferences() const;

      EntityFactory::Ptr getEntityFactory() const;
      void setEntityFactory(const EntityFactory::Ptr &entityFactory);

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
      QVariant getFlatPropertyValue(const QSharedPointer<T> &item, const QString &prop, ConverterStore::Ptr converterStore) const {
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
      friend class MetaEntityBuilder;
      MetaEntity();

      QString m_source;
      QPair<QString, QString> m_key;
      QHash<QString, MetaProperty> m_propertyMapping;

      KeyGenerationStrategy m_keyGenerationstrategy;
      QString m_sequence;

      EntityFactory::Ptr m_entityFactory;
   };

   namespace Mappings {
      template <class T> MetaEntity::Ptr mapping() {}
   }
}

Q_DECLARE_METATYPE(QSharedPointer<QObject>)
