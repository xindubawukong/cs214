cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

include(CTest)
enable_testing()

aux_source_directory(. SOURCE_FILES)
add_executable(main ${SOURCE_FILES})

find_package(Threads REQUIRED)
find_package(Parlay CONFIG REQUIRED)

target_link_libraries(main PRIVATE Parlay::parlay)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
