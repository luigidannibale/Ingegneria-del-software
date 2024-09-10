cmake_minimum_required(VERSION 3.0)
project(Server)

set(CMAKE_CXX_STANDARD 20)
set(SOURCES
        src/Server.cpp
        src/Server.hpp
        src/RedisManager.cpp
        src/RedisManager.hpp
        src/Database.cpp
        src/Database.hpp
        src/Test.cpp
        src/Test.hpp
)
add_executable(Server ${SOURCES})

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(PostgreSQL REQUIRED)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIBRARY NAMES hiredis)

target_include_directories(Server PRIVATE ${PostgreSQL_INCLUDE_DIRS} ${HIREDIS_INCLUDE_DIR})
target_link_libraries(Server PRIVATE ${PostgreSQL_LIBRARIES} ${HIREDIS_LIBRARY})
target_link_libraries(Server PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Server PRIVATE -lhiredis -lpq)