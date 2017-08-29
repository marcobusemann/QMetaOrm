include(../cmake/qtest.cmake)
set(TESTNAME QormSessionFirebirdIT)
set(SRC
    DatabaseFactory.h
    Person.cpp
    Person.h
    SqlHelper.h
    TestMacros.h
    QormDefaultSessionFirebirdIT.cpp
    resource.qrc)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)
set(EXTRA_INCLUDEPATHS
	../shared)
include(../cmake/base.cmake)
