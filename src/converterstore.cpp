#include <QMetaOrm/converterstore.h>

using namespace QMetaOrm;

//-----------------------------------------------------------------------------
DefaultConverterStore::Ptr DefaultConverterStore::factory()
{
	return DefaultConverterStore::Ptr(new DefaultConverterStore());
}

//-----------------------------------------------------------------------------
void DefaultConverterStore::registerConverter(Converter::Ptr converter)
{
	registerConverterBy(converter->getName(), converter);
}

//-----------------------------------------------------------------------------
void DefaultConverterStore::registerConverterBy(const QString &name, Converter::Ptr converter) {
   // TODO: Notify if already something registered?
   m_converters[name] = converter;
}

//-----------------------------------------------------------------------------
Converter::Ptr DefaultConverterStore::getConverterFor(const QString &name) const {
   Converter::Ptr result;
   if (m_converters.contains(name)) {
      result = m_converters[name];
   }
   return result;
}

//-----------------------------------------------------------------------------
bool DefaultConverterStore::hasConverter(const QString &name) const {
   return m_converters.contains(name);
}
