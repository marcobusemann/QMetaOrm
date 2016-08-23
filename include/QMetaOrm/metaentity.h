#pragma once

#include <QMetaOrm/private.h>

#include <qvariant.h>
#include <qhash.h>
#include <qpair.h>
#include <qlist.h>

/**
 * @brief The MetaEntity struct
 */
struct QMETAORM_LIBRARY_API MetaEntity
{
   static const QList<QVariant::Type> SupportedKeyTypes;

   QString source;
   QString sequence;
   QPair<QString, QString> key;
   QHash<QString, QString> propertyMapping;

   bool isValid() {
      return !key.first.isEmpty() || !propertyMapping.isEmpty();
   }

   template <class T>
   bool hasValidKey(const T &item) const {
      auto keyValue = getProperty(item, key.first);
      if (!keyValue.isValid()) return false;
      return
         keyValue.type() == QVariant::Int ? keyValue.toInt() > 0 :
         keyValue.type() == QVariant::LongLong ? keyValue.toLongLong() > 0 :
         keyValue.type() == QVariant::String ? !keyValue.toString().isEmpty() : false;
   }

   template <class T>
   QVariant getProperty(const T &item, const QString &name) const {
      return item.property(name.toStdString().c_str());
   }

   template <class T>
   void setProperty(T &item, const QString &name, const QVariant &value) const {
      item.setProperty(name.toStdString().c_str(), value);
   }
};

namespace QMetaOrmMappings {
   template <class T> MetaEntity mapping() {}
}

