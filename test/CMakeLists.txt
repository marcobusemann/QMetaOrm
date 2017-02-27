add_subdirectory(EntityMapperTest)
add_subdirectory(MetaEntityTest)
add_subdirectory(SessionIT)

set_property(TARGET EntityMapperTest PROPERTY FOLDER "tests")
set_property(TARGET MetaEntityTest PROPERTY FOLDER "tests")
set_property(TARGET SessionIT PROPERTY FOLDER "tests")