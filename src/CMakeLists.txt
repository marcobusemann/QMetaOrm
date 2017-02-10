set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../include)

set(header_path ../include/QMetaOrm)
set(header
    ${header_path}/Private.h
    ${header_path}/MetaEntity.h
    ${header_path}/MetaEntityBuilder.h
    ${header_path}/Criterion.h
    ${header_path}/ValueCriterion.h
    ${header_path}/ListCriterion.h
    ${header_path}/EntitySqlBuilder.h
    ${header_path}/EntityMapper.h
    ${header_path}/Exceptions.h
    ${header_path}/Session.h
    ${header_path}/DatabaseFactory.h
    ${header_path}/SessionFactory.h
    ${header_path}/Converter.h
    ${header_path}/ConverterStore.h
    ${header_path}/Logger.h
    ${header_path}/Cache.h
    ${header_path}/CacheFactory.h)

set(src
    MetaEntityBuilder.cpp
    Criterion.cpp
    ValueCriterion.cpp
    ListCriterion.cpp
    EntitySqlBuilder.cpp
    Session.cpp
    SessionFactory.cpp
    MetaEntity.cpp
    ConverterStore.cpp
    EntityMapper.cpp
    Logger.cpp
    Cache.cpp
    CacheFactory.cpp)

# Bind to qt
find_package(Qt5Core)
find_package(Qt5Sql)

if(Qt5Core_FOUND AND Qt5Sql_FOUND)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt5::Core Qt5::Sql)    
else()
    find_package(Qt4 REQUIRED)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt4::Core Qt4::Sql)
endif()

add_library(QMetaOrm ${header} ${src})
target_include_directories(QMetaOrm PUBLIC ${QMETAORM_SOURCE_DIR}/include
										   ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(QMetaOrm ${EXTRA_LIBS})