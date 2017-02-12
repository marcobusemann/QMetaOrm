set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test)

if(Qt5Test_FOUND)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt5::Test)    
else()
    find_package(Qt4 REQUIRED)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt4::Test)
endif()