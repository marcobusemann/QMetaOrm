set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(header_path_include ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(header_path ${CMAKE_CURRENT_SOURCE_DIR}/../include/QMetaOrm)

include_directories(${header_path_include})

add_definitions(-DQMETAORM_LIBRARY)

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
    find_package(Qt4 REQUIRED QtCore QtSql)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt4::QtCore Qt4::QtSql)
endif()

set(needed_features
    cxx_strong_enums
    cxx_constexpr
    cxx_auto_type
    cxx_nullptr
    cxx_override
    cxx_lambdas)

add_library(QMetaOrm SHARED ${header} ${src})
target_compile_features(QMetaOrm PRIVATE ${needed_features})
target_include_directories(QMetaOrm PUBLIC ${header_path_include}
										   ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(QMetaOrm ${EXTRA_LIBS})