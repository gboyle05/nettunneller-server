cmake_minimum_required(VERSION 3.15)
project(nettunneller-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(nettunneller-server main.cpp
        temp2.cpp)
