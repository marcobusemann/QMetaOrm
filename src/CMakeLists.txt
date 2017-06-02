set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(header_path_include ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(header_path ${CMAKE_CURRENT_SOURCE_DIR}/../include/QMetaOrm)

include_directories(${header_path_include})

add_definitions(-DQMETAORM_LIBRARY)

set(header
    ${header_path}/QormPrivate.h
    ${header_path}/QormMetaEntity.h
    ${header_path}/QormMetaEntityBuilder.h
    ${header_path}/QormCriterion.h
    ${header_path}/QormValueCriterion.h
    ${header_path}/QormListCriterion.h
    ${header_path}/QormEntitySqlBuilder.h
    ${header_path}/QormEntityMapper.h
    ${header_path}/QormExceptions.h
    ${header_path}/QormSession.h
    ${header_path}/QormDatabaseFactory.h
    ${header_path}/QormSessionFactory.h
    ${header_path}/QormConverter.h
    ${header_path}/QormConverterStore.h
    ${header_path}/QormLogger.h
    ${header_path}/QormCache.h
    ${header_path}/QormCacheFactory.h)

set(src
    QormMetaEntityBuilder.cpp
    QormCriterion.cpp
    QormValueCriterion.cpp
    QormListCriterion.cpp
    QormEntitySqlBuilder.cpp
    QormSession.cpp
    QormSessionFactory.cpp
    QormMetaEntity.cpp
    QormConverterStore.cpp
    QormEntityMapper.cpp
    QormLogger.cpp
    QormCache.cpp
    QormCacheFactory.cpp)

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