cmake_minimum_required(VERSION 3.1)

project(QMetaOrm)

# The version number.
set (QMETAORM_VERSION_MAJOR 0)
set (QMETAORM_VERSION_MINOR 1)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

enable_testing()

add_subdirectory(src)
add_subdirectory(test)