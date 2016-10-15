#include <QMetaOrm/converterstore.h>

using namespace QMetaOrm;

//-----------------------------------------------------------------------------
void DefaultConverterStore::registerBy(const QString &name, Converter::Ptr converter) {
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
