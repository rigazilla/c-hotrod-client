cmake_minimum_required (VERSION 2.6)
project (c-client)

include_directories ("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/ext/usr/include")
find_library(HOTROD_LIBRARY NAME hotrod HINTS "${PROJECT_SOURCE_DIR}/ext/usr/lib")
add_library(chotrod src/chotrod.cpp)
target_link_libraries(chotrod ${HOTROD_LIBRARY})
add_executable(example test/example.c)
target_link_libraries (example chotrod)



