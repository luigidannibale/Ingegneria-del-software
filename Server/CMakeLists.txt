cmake_minimum_required(VERSION 3.29)
project(Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(Server
        server.cpp
        Server.cpp
        Server.hpp
        RedisManager.cpp
        RedisManager.hpp)
