#pragma once

#include <QMetaOrm/Converter.h>
#include <QMetaOrm/Private.h>
#include <qsharedpointer.h>

namespace QMetaOrm {

   /**
   * @brief The ConverterStore class
   */
   class QMETAORM_LIBRARY_API ConverterStore
   {
   public:
      typedef QSharedPointer<ConverterStore> Ptr;

   public:
	  virtual void registerConverter(Converter::Ptr converter) = 0;
	  virtual void registerConverterBy(const QString &name, Converter::Ptr converter) = 0;
      virtual Converter::Ptr getConverterFor(const QString &name) const = 0;
      virtual bool hasConverter(const QString &name) const = 0;
   };

   /**
   * @brief The DefaultConverterStore class
   */
   class QMETAORM_LIBRARY_API DefaultConverterStore : public ConverterStore
   {
   public:
      typedef QSharedPointer<DefaultConverterStore> Ptr;
	  static Ptr factory();

   public:
	  virtual void registerConverter(Converter::Ptr converter) override;
	  virtual void registerConverterBy(const QString &name, Converter::Ptr converter) override;
      virtual Converter::Ptr getConverterFor(const QString &name) const override;
      virtual bool hasConverter(const QString &name) const override;

   private:
      QHash<QString, Converter::Ptr> m_converters;
   };
}