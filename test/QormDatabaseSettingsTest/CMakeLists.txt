include(../cmake/qtest.cmake)
set(TESTNAME QormDatabaseSettingsTest)
set(SRC ${TESTNAME}.cpp)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)
include(../cmake/base.cmake)
