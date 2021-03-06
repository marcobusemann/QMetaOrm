set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core)
find_package(Qt5Test)
find_package(Qt5Sql)

if(Qt5Core_FOUND AND Qt5Sql_FOUND AND Qt5Test_FOUND)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt5::Core Qt5::Sql Qt5::Test)    
else()
    find_package(Qt4 REQUIRED QtCore QtSql QtTest)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt4::QtCore Qt4::QtSql Qt4::QtTest)
endif()