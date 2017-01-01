#include <QMetaOrm/ConverterStore.h>
#include <QDebug>

using namespace QMetaOrm;

DefaultConverterStore::Ptr DefaultConverterStore::factory()
{
	return DefaultConverterStore::Ptr(new DefaultConverterStore());
}

void DefaultConverterStore::registerConverter(const QString &name, const Converter::Ptr &converter) {
   if (m_converters.contains(name))
       qDebug() << "A converter for " << name << " has already been registered. It will be overwritten!";
   m_converters[name] = converter;
}

Converter::Ptr DefaultConverterStore::getConverterFor(const QString &name) const {
   Converter::Ptr result;
   if (m_converters.contains(name)) {
      result = m_converters[name];
   }
   return result;
}

bool DefaultConverterStore::hasConverter(const QString &name) const {
   return m_converters.contains(name);
}
