include(../cmake/qtest.cmake)
set(TESTNAME QormSessionSqliteIT)
set(SRC
    DatabaseFactory.h
    QormDefaultSessionSqliteIT.cpp
	../shared/Person.cpp
	../shared/QormDefaultSessionIT.h
    resource.qrc)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)
set(EXTRA_INCLUDEPATHS
	../shared)
include(../cmake/base.cmake)
