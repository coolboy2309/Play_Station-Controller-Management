cmake_minimum_required(VERSION 3.20)

project(ZoneEyeAgent)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ZoneEyeAgent

    src/main.cpp
    src/Config.cpp
    src/Logger.cpp
)