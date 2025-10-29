cmake_minimum_required(VERSION 4.0)
project(FantasyConsole)

set(CMAKE_CXX_STANDARD 26)

add_executable(FantasyConsole main.cpp
        CpuState.h
        System.h
        System.cpp
        OpcodeHandler.cpp
        OpcodeHandler.h)
