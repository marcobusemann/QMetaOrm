#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm\converterstore.h>

#include <qsharedpointer.h>
#include <qstringlist.h>
#include <qvariant.h>
#include <qhash.h>
#include <qpair.h>
#include <qlist.h>

#include <functional>

namespace QMetaOrm {

   /**
   * @brief The MetaProperty struct
   */
   struct MetaProperty 
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

   /**
    * @brief The MetaEntity struct
    */
   class QMETAORM_LIBRARY_API MetaEntity
   {
   public:
      typedef std::function<QVariant(const QSharedPointer<QObject> &)> ReferenceCaster;
      typedef std::function<QSharedPointer<QObject> (const QVariant &)> VariantToReferenceCaster;
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

      void setKey(const QString &aProperty, const QString &aDatabaseField);
      const QString &getKeyProperty() const;
      const QString &getKeyDatabaseField() const;
      bool hasKey() const;

      QList<QString> getProperties() const;
      const MetaProperty &getProperty(const QString &aProperty);
      void addProperty(const MetaProperty &prop);

      QList<MetaProperty> getReferences() const;

      QSharedPointer<QObject> createReferenceObject() const;

      ReferenceCaster getReferenceCaster() const;
      void setReferenceCaster(ReferenceCaster func);

      VariantToReferenceCaster getVariantToReferenceCaster() const;
      void setVariantToReferenceCaster(VariantToReferenceCaster func);
      
      ObjectConstructor getObjectConstructor() const;
	  void setObjectConstructor(ObjectConstructor constructor);

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

      template <class T>
      QVariant getProperty(const QSharedPointer<T> &item, const QString &name) const {
         return item->property(name.toStdString().c_str());
      }

      template <class T>
      QVariant getFlatPropertyValue(const QSharedPointer<T> &item, const QString &prop, ConverterStore::Ptr converterStore) const {
         auto propMeta = m_propertyMapping[prop];
         QVariant result;
         if (propMeta.isReference())
         {
            if (m_variantToReferenceCaster == nullptr)
               result = QVariant();
            else {
               auto refItemVariant = getProperty(item, prop);
               auto refItem = m_variantToReferenceCaster(refItemVariant);
               auto keyProperty = propMeta.reference->getKeyProperty();
               result = refItem == nullptr ? QVariant() : refItem->property(keyProperty.toStdString().c_str());
            }
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
      MetaEntity() {}

      QString m_source;
      QString m_sequence;
      QPair<QString, QString> m_key;
      QHash<QString, MetaProperty> m_propertyMapping;
	  ObjectConstructor m_objectConstructor;
      ReferenceCaster m_referenceCaster;
      VariantToReferenceCaster m_variantToReferenceCaster;
   };

   namespace Mappings {
      template <class T> MetaEntity::Ptr mapping() {}
   }
}

