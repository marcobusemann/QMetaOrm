cmake_minimum_required(VERSION 3.1)

project(QMetaOrm)

# The version number.
set (QMETAORM_VERSION_MAJOR 0)
set (QMETAORM_VERSION_MINOR 1)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)