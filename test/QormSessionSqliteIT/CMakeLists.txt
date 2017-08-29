include(../cmake/qtest.cmake)
set(TESTNAME QormSessionIT)
set(SRC
    DatabaseFactory.h
    Person.cpp
    Person.h
    SqlHelper.h
    TestMacros.h
    QormDefaultSessionIT.cpp
    resource.qrc)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)
include(../cmake/base.cmake)
