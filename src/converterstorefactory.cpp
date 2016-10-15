#include <QMetaOrm\converterstorefactory.h>

using namespace QMetaOrm;

//-----------------------------------------------------------------------------
DefaultConverterStoreFactory::Ptr DefaultConverterStoreFactory::factory() {
   return DefaultConverterStoreFactory::Ptr(new DefaultConverterStoreFactory());
}

//-----------------------------------------------------------------------------
DefaultConverterStoreFactory::DefaultConverterStoreFactory()
   : m_store(DefaultConverterStore::Ptr(new DefaultConverterStore()))
{

}

//-----------------------------------------------------------------------------
ConverterStore::Ptr DefaultConverterStoreFactory::createConverterStore() const
{
   return m_store;
}
