SET(WITH_FIREBIRD_TESTS OFF CACHE BOOL "Use this option if a Firebird server is available within your environment")

add_subdirectory(QormEntityMapperTest)
add_subdirectory(QormMetaEntityTest)
add_subdirectory(QormDatabaseSettingsTest)
add_subdirectory(QormSessionSqliteIT)

set_property(TARGET QormEntityMapperTest PROPERTY FOLDER "tests")
set_property(TARGET QormMetaEntityTest PROPERTY FOLDER "tests")
set_property(TARGET QormDatabaseSettingsTest PROPERTY FOLDER "tests")
set_property(TARGET QormSessionSqliteIT PROPERTY FOLDER "tests")

if (${WITH_FIREBIRD_TESTS})
	add_subdirectory(QormSessionFirebirdIT)

	set_property(TARGET QormSessionFirebirdIT PROPERTY FOLDER "tests")
endif (${WITH_FIREBIRD_TESTS})