include(../cmake/qtest.cmake)
set(TESTNAME QormEntityMapperTest)
set(SRC ${TESTNAME}.cpp
        ../../src/QormEntityMapper.h
        ../../src/QormEntityMapper.cpp)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)
include(../cmake/base.cmake)
