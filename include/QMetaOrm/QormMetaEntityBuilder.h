#pragma once

#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QMetaProperty>
#include <QDebug>

class QMETAORM_LIBRARY_API QormMetaEntityBuilder
{
public:
   static QormMetaEntityBuilder anEntity();
   static QormMetaEntityBuilder derivedEntity(const QormMetaEntity::Ptr &aParentEntity);
   QormMetaEntityBuilder(const QormMetaEntity::Ptr &aParentEntity);
   QormMetaEntityBuilder();
   QormMetaEntityBuilder forSource(const QString &source);
   QormMetaEntityBuilder withSequence(const QString &sequence);
   QormMetaEntityBuilder withId(const QString &prop, const QString &field);
   QormMetaEntityBuilder withData(const QString &prop, const QString &field, const QString &converter = QString());
   QormMetaEntityBuilder withOneToMany(const QString &prop, const QString &field, QormMetaEntity::Ptr referenceEntity);
   QormMetaEntityBuilder withEntityFactory(const QormEntityFactory::Ptr &entityFactory);

   template <class T>
   QormMetaEntity::Ptr build() const {
      Q_ASSERT_X(m_entity->hasSource(), "build", "source not specified");
      Q_ASSERT_X(m_entity->hasKey(), "build", "key not specified");

      auto keyType = gatherKeyType<T>(m_entity);
      Q_ASSERT_X(QormMetaEntity::SupportedKeyTypes.contains(keyType), "build", "actually only int, long or string key types are allowed!");

      m_entity->setEntityFactory(QormEntityFactory::Ptr(new QormDefaultEntityFactory<T>()));

      return m_entity->copy();
   }

private:
   template <class T>
   static QVariant::Type gatherKeyType(QormMetaEntity::Ptr mapping) {
      // TODO: error handling
      QMetaObject metaObject = T::staticMetaObject;
      int keyPropertyIndex = metaObject.indexOfProperty(mapping->getKeyProperty().toStdString().c_str());
      QMetaProperty keyProperty = metaObject.property(keyPropertyIndex);
      return keyProperty.type();
   }

   QormMetaEntity::Ptr m_entity;
};
