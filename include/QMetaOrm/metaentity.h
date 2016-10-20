#pragma once

#include <QMetaOrm/private.h>

#include <qsharedpointer.h>
#include <qstringlist.h>
#include <qvariant.h>
#include <qhash.h>
#include <qpair.h>
#include <qlist.h>

namespace QMetaOrm {

   /**
   * @brief The MetaProperty struct
   */
   struct MetaProperty 
   {
      QString propertyName;
      QString databaseName;
      QString converterName;

      bool hasConverter() const {
         return !converterName.isEmpty();
      }
   };

   /**
    * @brief The MetaEntity struct
    */
   class QMETAORM_LIBRARY_API MetaEntity
   {
   public:
      typedef QSharedPointer<MetaEntity> Ptr;
      static Ptr factory() {
         return Ptr(new MetaEntity());
      }

   public:
      static const QList<QVariant::Type> SupportedKeyTypes;

      MetaEntity() {}
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

      template <class T>
      bool hasValidKey(const T &item) const {
         auto keyValue = getProperty(item, getKeyProperty());
         if (!keyValue.isValid()) return false;
         return
            keyValue.type() == QVariant::Int ? keyValue.toInt() > 0 :
            keyValue.type() == QVariant::LongLong ? keyValue.toLongLong() > 0 :
            keyValue.type() == QVariant::String ? !keyValue.toString().isEmpty() : false;
      }

      QStringList getDatabaseFields() const;

      template <class T>
      QVariant getProperty(const T &item, const QString &name) const {
         return item.property(name.toStdString().c_str());
      }

      template <class T>
      void setProperty(T &item, const QString &name, const QVariant &value) const {
         item.setProperty(name.toStdString().c_str(), value);
      }

      Ptr copy();

   private:
      QString m_source;
      QString m_sequence;
      QPair<QString, QString> m_key;
      QHash<QString, MetaProperty> m_propertyMapping;
   };

   namespace Mappings {
      template <class T> MetaEntity::Ptr mapping() {}
   }
}

