#include <QCoreApplication>
#include "main.h"
#include <QMetaOrm/metaentitybuilder.h>

#include <qtextcodec.h>

class Win1252Codec: public QTextCodec
{
public:
   virtual QByteArray name() const {
      return "WIN1252";
   }

   virtual int mibEnum() const {
      return 2252;
   }

   virtual QList<QByteArray> aliases() const {
      return QList<QByteArray>();
   }

protected:
   virtual QByteArray convertFromUnicode(const QChar * input, int number, ConverterState * state) const {
      auto codec = QTextCodec::codecForName("WINDOWS-1252");
      return codec->fromUnicode(input, number, state);
   }

   virtual QString convertToUnicode(const char * chars, int len, ConverterState * state) const {
      auto codec = QTextCodec::codecForName("WINDOWS-1252");
      return codec->toUnicode(chars, len, state);
   }
};

namespace QMetaOrmMappings {
   template <> MetaEntity mapping<Todo>()
   {
      static const MetaEntity map = MetaEntityBuilder::anEntity()
            .forSource("cal_todos")
            .withSequence("cal_items_gen")
            .withId(Todo::p::id, "id")
            .withData(Todo::p::title, "title")
            .withData(Todo::p::start, "todo_start")
            .withData(Todo::p::end, "todo_due")
            .build<Todo>();
      return map;
   }
}

const QString Todo::p::id = "id";
const QString Todo::p::title = "title";
const QString Todo::p::start = "start";
const QString Todo::p::end = "end";

#include <QMetaOrm/databasefactory.h>

class MyDatabaseFactory: public DatabaseFactory {
public:
    static DatabaseFactory::Ptr create() {
        return DatabaseFactory::Ptr(new MyDatabaseFactory());
    }

    virtual QSqlDatabase createDatabase() const {
        if (!QSqlDatabase::contains()) {
            Win1252Codec codec;

            QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
            db.setHostName("localhost");
            db.setDatabaseName("D:\\Development\\TreesoftOffice\\Version_65_msvc2012\\Database\\Demo\\DATA1.FDB");
            db.setUserName("sysdba");
            db.setPassword("masterkey");
            db.setConnectOptions("ISC_DPB_LC_CTYPE=WIN1252");
            return db;
        }
        else
            return QSqlDatabase::database();
    }
};

#include <QMetaOrm/sessionfactory.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try {
       auto databaseFactory = MyDatabaseFactory::create();
       auto sessionFactory = DefaultSessionFactory::create(databaseFactory);
       TodoRepository repo(sessionFactory);
       auto items = repo.find(TodoCriterionBuilder::aTodoWhere()
          .titleEquals("Treesoft ERP:")
          .idEquals(9391)
          .build(), 0, 2);

       foreach(auto item, items) {
          item.dump();
       }
    }
    catch (std::exception e) {
       qDebug() << e.what();
    }

    return a.exec();
}
