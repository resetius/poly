cmake_minimum_required(VERSION 3.14)
project(poly)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(COPY test DESTINATION .)
add_executable(poly poly.cpp)

add_test(NAME integration COMMAND /usr/bin/python3 -mpytest ${CMAKE_SOURCE_DIR}/test.py --junitxml=integration.xml)
