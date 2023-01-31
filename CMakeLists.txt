cmake_minimum_required(VERSION 3.12)

project(server CXX)
project(client CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g")
add_executable(server
        server/main.cpp
        common/UDPHandler.hpp common/RuntimeException.hpp common/ThreadSafeQueue.hpp)

add_executable(client
        client/main.cpp common/PackageManager.hpp common/RuntimeException.hpp common/ThreadSafeQueue.hpp)

target_link_libraries(server sfml-network)