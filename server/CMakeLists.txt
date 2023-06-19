cmake_minimum_required(VERSION 3.18.4)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(server main.cpp ClientConnection.cpp ClientConnection.h Server.cpp Server.h ConnectionManager.cpp ConnectionManager.h)

target_link_libraries(server pthread)