cmake_minimum_required(VERSION 4.0)
project(FantasyConsole)

set(CMAKE_CXX_STANDARD 26)

file(GLOB SOURCES "src/*.cpp")

add_executable(FantasyConsole ${SOURCES})
