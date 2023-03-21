
cmake_minimum_required(VERSION 3.23)
project(PageTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(PageTree main.cpp vaddr_tracereader.h vaddr_tracereader.c
        Level.h Level.cpp bitManipulation.cpp bitManipulation.h Map.cpp Map.h PageTable.h
        PageTable.cpp TLB.cpp TLB.h print_helpers.c print_helpers.h PageTableStats.cpp PageTableStats.h)
