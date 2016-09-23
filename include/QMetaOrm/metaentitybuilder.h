#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/metaentity.h>
#include <QMetaProperty>

namespace QMetaOrm {

   /**
    * @brief The MetaEntityBuilder class
    */
   class QMETAORM_LIBRARY_API MetaEntityBuilder
   {
   public:
      static MetaEntityBuilder anEntity();
      MetaEntityBuilder forSource(const QString &source);
      MetaEntityBuilder withSequence(const QString &sequence);
      MetaEntityBuilder withId(const QString &prop, const QString &field);
      MetaEntityBuilder withData(const QString &prop, const QString &field);

      template <class T>
      MetaEntity build() const {
         Q_ASSERT_X(!m_entity.source.isEmpty(), "build", "source not specified");
         Q_ASSERT_X(!m_entity.key.first.isEmpty(), "build", "key not specified");

         auto keyType = gatherKeyType<T>(m_entity);
         Q_ASSERT_X(MetaEntity::SupportedKeyTypes.contains(keyType), "build", "actually only int, long or string key types are allowed!");

         // TODO: Add futher property checks

         return m_entity;
      }

   private:
      template <class T>
      static QVariant::Type gatherKeyType(MetaEntity mapping) {
         // TODO: error handling
         QMetaObject metaObject = T::staticMetaObject;
         int keyPropertyIndex = metaObject.indexOfProperty(mapping.key.first.toStdString().c_str());
         QMetaProperty keyProperty = metaObject.property(keyPropertyIndex);
         return keyProperty.type();
      }

      MetaEntity m_entity;
   };
}
