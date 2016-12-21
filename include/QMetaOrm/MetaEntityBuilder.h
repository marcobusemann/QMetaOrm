#pragma once

#include <QMetaOrm/Private.h>
#include <QMetaOrm/MetaEntity.h>
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
      MetaEntityBuilder withOneToMany(const QString &prop, const QString &field, MetaEntity::Ptr referenceEntity);
      MetaEntityBuilder withEntityFactory(const EntityFactory::Ptr &entityFactory);
      MetaEntityBuilder withEmbeddedPtrNamingScheme();

      template <class T>
      MetaEntity::Ptr build() const {
         Q_ASSERT_X(m_entity->hasSource(), "build", "source not specified");
         Q_ASSERT_X(m_entity->hasKey(), "build", "key not specified");

         auto keyType = gatherKeyType<T>(m_entity);
         Q_ASSERT_X(MetaEntity::SupportedKeyTypes.contains(keyType), "build", "actually only int, long or string key types are allowed!");

         m_entity->setEntityFactory(EntityFactory::Ptr(new DefaultEntityFactory<T>()));

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
      bool m_useEmbeddedPtrEntityFactoryNamingScheme;
   };
}
