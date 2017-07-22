#pragma once

#include <QMetaOrm/QormMetaEntity.h>
#include <QMetaOrm/QormConverter.h>
#include <QMetaOrm/QormPrivate.h>

#include <QSharedPointer>
#include <QMetaProperty>

#include <functional>

class QMETAORM_LIBRARY_API QormMetaEntityBuilder {
public:
    static QormMetaEntityBuilder anEntity();

    static QormMetaEntityBuilder derivedEntity(const QormMetaEntity::Ptr& aParentEntity);

    QormMetaEntityBuilder(const QormMetaEntity::Ptr& aParentEntity);

    QormMetaEntityBuilder();

    QormMetaEntityBuilder forSource(const QString& source);

    QormMetaEntityBuilder withSequence(const QString& sequence);

    QormMetaEntityBuilder withId(const QString& prop, const QString& field);

    QormMetaEntityBuilder withData(const QString& prop, const QString& field);

    QormMetaEntityBuilder
    withConvertedData(const QString& prop, const QString& field, std::function<QormConverter::Ptr()> converterSelector);

    QormMetaEntityBuilder
    withConvertedData(const QString& prop, const QString& field, const QormConverter::Ptr& converter);

    template<class ConverterType>
    QormMetaEntityBuilder withConvertedData(const QString& prop, const QString& field)
    {
        return withConvertedData(prop, field, []() {
            return QSharedPointer<ConverterType>(new ConverterType());
        });
    }

    QormMetaEntityBuilder withOneToMany(const QString& prop, const QString& field, QormMetaEntity::Ptr referenceEntity);

    QormMetaEntityBuilder withEntityFactory(const QormEntityFactory::Ptr& entityFactory);

    template<class T>
    QormMetaEntity::Ptr build() const
    {
        Q_ASSERT_X(m_entity->hasSource(), "build", "source not specified");
        Q_ASSERT_X(m_entity->hasKey(), "build", "key not specified");

        auto keyType = gatherKeyType<T>(m_entity);
        Q_ASSERT_X(QormMetaEntity::SupportedKeyTypes.contains(keyType), "build",
            "actually only int, long or string key types are allowed!");

        m_entity->setEntityFactory(QormEntityFactory::Ptr(new QormDefaultEntityFactory<T>()));

        return m_entity->copy();
    }

private:
    template<class T>
    static QVariant::Type gatherKeyType(QormMetaEntity::Ptr mapping)
    {
        QMetaObject metaObject = T::staticMetaObject;
        int keyPropertyIndex = findKeyPropertyIndex(&metaObject, mapping->getKeyProperty().toStdString().c_str());
        return keyPropertyIndex<0 ? QVariant::Type::Invalid : metaObject.property(keyPropertyIndex).type();
    }

    static int findKeyPropertyIndex(const QMetaObject* metaObject, const QString& prop)
    {
        if (metaObject==nullptr)
            return -1;
        auto keyPropertyIndex = metaObject->indexOfProperty(prop.toStdString().c_str());
        return keyPropertyIndex<0 ? findKeyPropertyIndex(metaObject->superClass(), prop) : keyPropertyIndex;
    }

    QormMetaEntity::Ptr m_entity;
};
