#pragma once

#include <QMetaOrm/private.h>
#include <QMetaOrm/metaentity.h>
#include <QMetaProperty>
#include <QSharedPointer>
#include <QDebug>

namespace QMetaOrm {

   /**
    * @brief The MetaEntityBuilder class
    */
   class QMETAORM_LIBRARY_API MetaEntityBuilder
   {
   public:
      static MetaEntityBuilder anEntity();
      MetaEntityBuilder();
      MetaEntityBuilder forSource(const QString &source);
      MetaEntityBuilder withSequence(const QString &sequence);
      MetaEntityBuilder withId(const QString &prop, const QString &field);
      MetaEntityBuilder withData(const QString &prop, const QString &field, const QString &converter = QString());
      MetaEntityBuilder withReference(const QString &prop, const QString &field, MetaEntity::Ptr referenceEntity);
      MetaEntityBuilder withReferenceCaster(MetaEntity::ReferenceCaster func);

      template <class T>
      MetaEntity::Ptr build() const {
         Q_ASSERT_X(m_entity->hasSource(), "build", "source not specified");
         Q_ASSERT_X(m_entity->hasKey(), "build", "key not specified");

         auto keyType = gatherKeyType<T>(m_entity);
         Q_ASSERT_X(MetaEntity::SupportedKeyTypes.contains(keyType), "build", "actually only int, long or string key types are allowed!");

         // TODO: Add futher property checks

         m_entity->setObjectConstructor([]() -> QObject* {
            return new T();
         });

         if (m_entity->getReferenceCaster() == nullptr)
            m_entity->setReferenceCaster([](const QSharedPointer<QObject> &obj) -> QVariant {
            return QVariant::fromValue(obj.objectCast<T>());
         });
         if (m_entity->getVariantToReferenceCaster() == nullptr)
            m_entity->setVariantToReferenceCaster([](const QVariant &value) -> QSharedPointer<QObject> {
            QSharedPointer<QObject> result;
            qDebug() << value.userType();
            result = *reinterpret_cast<const T::Ptr *>(value.constData());
            if (result == nullptr)
               result = *reinterpret_cast<const QSharedPointer<T> *>(value.constData());
            return result;
         });

         qRegisterMetaType<QSharedPointer<T>>();
         qRegisterMetaType<T::Ptr>();
         qRegisterMetaType<T>();

         return m_entity->copy();
      }

   private:
      template <class T>
      static QVariant::Type gatherKeyType(MetaEntity::Ptr mapping) {
         // TODO: error handling
         QMetaObject metaObject = T::staticMetaObject;
         int keyPropertyIndex = metaObject.indexOfProperty(mapping->getKeyProperty().toStdString().c_str());
         QMetaProperty keyProperty = metaObject.property(keyPropertyIndex);
         return keyProperty.type();
      }

      MetaEntity::Ptr m_entity;
   };
}
