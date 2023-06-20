cmake_minimum_required(VERSION 3.18.4)
project(client)

set(CMAKE_CXX_STANDARD 14)

add_executable(client main.cpp Client.cpp Client.h)

target_link_libraries(client pthread)