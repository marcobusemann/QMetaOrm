include(../cmake/qtest.cmake)
set(TESTNAME SessionIT)
set(SRC
    DatabaseFactory.h
    Person.cpp
    Person.h
    SessionIT.cpp
    resource.qrc)
include(../cmake/base.cmake)
