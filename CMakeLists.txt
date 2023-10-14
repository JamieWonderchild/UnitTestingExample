cmake_minimum_required(VERSION 3.26)
project(TestingProject)

set(CMAKE_CXX_STANDARD 23)

add_executable(TestingProject main.cpp
        include/AutoBreak.h
        test/UnitTests.hpp
        test/TestCases.hpp)
