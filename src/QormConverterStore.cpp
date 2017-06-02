#include <QMetaOrm/QormConverterStore.h>
#include <QDebug>

QormDefaultConverterStore::Ptr QormDefaultConverterStore::factory()
{
   return QormDefaultConverterStore::Ptr(new QormDefaultConverterStore());
}

void QormDefaultConverterStore::registerConverter(const QString &name, const QormConverter::Ptr &converter) {
   if (m_converters.contains(name))
      qDebug() << "A converter for " << name << " has already been registered. It will be overwritten!";
   m_converters[name] = converter;
}

QormConverter::Ptr QormDefaultConverterStore::getConverterFor(const QString &name) const {
   QormConverter::Ptr result;
   if (m_converters.contains(name)) {
      result = m_converters[name];
   }
   return result;
}

bool QormDefaultConverterStore::hasConverter(const QString &name) const {
   return m_converters.contains(name);
}
