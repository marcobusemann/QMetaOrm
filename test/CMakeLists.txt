add_subdirectory(QormEntityMapperTest)
add_subdirectory(QormMetaEntityTest)
add_subdirectory(QormSessionIT)

set_property(TARGET QormEntityMapperTest PROPERTY FOLDER "tests")
set_property(TARGET QormMetaEntityTest PROPERTY FOLDER "tests")
set_property(TARGET QormSessionIT PROPERTY FOLDER "tests")