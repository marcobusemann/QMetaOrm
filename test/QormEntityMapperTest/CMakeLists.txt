include(../cmake/qtest.cmake)
set(TESTNAME QormEntityMapperTest)
set(SRC ${TESTNAME}.cpp ../../src/QormStandardQtLogger.h ../../src/QormStandardQtLogger.cpp)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)
include(../cmake/base.cmake)
