# NODE, Project has been canceled. As this ORM is fully based on the Qt-Meta-Object system, you loose a lot of flexibility when designing your models. They are fully Qt dependent and need getter/setters wich lead to anemic domain models. There will be no more development on this project.

Platform    | Status | 
----------- | :------------ |
Windows | [![Windows Status](https://ci.appveyor.com/api/projects/status/2u12ikvskeoi5fjy?svg=true)](https://ci.appveyor.com/project/marcobusemann/qmetaorm)
Linux & OSX | [![Linux & Osx Status](https://travis-ci.org/marcobusemann/QMetaOrm.svg?branch=master)](https://travis-ci.org/marcobusemann/QMetaOrm)

# Getting started
A simple usage for sqlite looks like this:
```cpp
auto settings = QormDatabaseSettings();
settings.setDatabaseName("file.sqlite");

auto sessionFactory = QormSessionFactoryBuilder::AFactory()
    .withDatabase("QSQLITE", settings)
    .build();
    
auto session = sessionFactory->createSession();
auto address = session->selectOne<Address>("select * from address where id = ?", QVariantList() << 1);
```

# Building the library
## Unix
```
mkdir build && cd build
cmake ..
make
ctest -V
```

# Style-Guide
http://www.stroustrup.com/Programming/PPP-style-rev3.pdf

# Work in progress

# Just some notes

## Todos
- Support multiple database systems by checking if features are available.
  - Eg. the "returning" keyword in create statements.
- How to support different styles of shared pointers? (std::shared_ptr, QSharedPointer, ...)
- How to handle nullable fields?

It shoult be possible to use either one-to-many references with embedded types or just store the referenced ids directly. When doing the latter, how can i descide whether to write a null or a valid value? Without having a special type like boost::optional this is only possible through a special converter which checks for eg. 0 or -1 and then returns an invalid QVariant instead. Can we use boost::optional in combination with Qts-Meta-System?

- Update only fields which have changed

How can we do this without adding logik to the domain type? Actually the domain type is based on the Qt-Meta-System only. It does not have any other dependencies nor it contains orm specific logik.

- Value-Conversion

Actually we have a converter which could be referenced by name in the mapping configuration.
Instead it would be nice to have default mappings from INTEGER to QString which will be applied when those combinations occur.
