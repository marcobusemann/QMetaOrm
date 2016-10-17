#pragma once

#include <QMetaOrm\private.h>
#include <qsharedpointer.h>
#include <qvariant.h>

namespace QMetaOrm {

   class QMETAORM_LIBRARY_API Converter {
   public:
      typedef QSharedPointer<Converter> Ptr;

   public:
      virtual ~Converter() {}

	  virtual QString getName() const = 0;
	  virtual QVariant convert(const QVariant &value) const = 0;
   };
}