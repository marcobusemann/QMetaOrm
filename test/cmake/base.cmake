add_executable(${TESTNAME} ${SRC})
target_link_libraries(${TESTNAME} QMetaOrm ${EXTRA_LIBS})
target_include_directories(${TESTNAME} PRIVATE
                            ${CMAKE_CURRENT_BINARY_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_test(${TESTNAME} ${TESTNAME})