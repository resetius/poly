cmake_minimum_required(VERSION 3.14)
project(poly)
enable_testing()

find_package(PkgConfig REQUIRED)
pkg_check_modules(CMOCKA cmocka REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(COPY test DESTINATION .)
add_executable(poly poly.cpp)
add_executable(ut test.cpp)

target_include_directories(ut PRIVATE ${CMOCKA_INCLUDE_DIRS})
target_link_directories(ut PRIVATE ${CMOCKA_LIBRARY_DIRS})
target_link_libraries(ut ${CMOCKA_LIBRARIES})

add_test(NAME ut COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ut)
set_tests_properties(ut PROPERTIES ENVIRONMENT "CMOCKA_MESSAGE_OUTPUT=xml;CMOCKA_XML_FILE=ut.xml")

add_test(NAME integration COMMAND /usr/bin/python3 -mpytest ${CMAKE_SOURCE_DIR}/test.py --junitxml=integration.xml)
