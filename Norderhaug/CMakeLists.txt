cmake_minimum_required(VERSION 3.21)
project(Norderhaug)

set(CMAKE_CXX_STANDARD 11)

include_directories(${APP_DOMAIN})

set(
    SOURCE_FILES
        print_helpers.h
        print_helpers.c
        vaddr_tracereader.h
        vaddr_tracereader.c
        main.cpp
        bitManipulation.cpp
        Map.cpp
        Map.h
        PageTable.cpp
        PageTable.h
        PageTableStats.cpp
        PageTableStats.h
        Level.cpp
        Level.h
        TLB.cpp
        TLB.h
        ArgParser.cpp
        ArgParser.h
        AddressConstants.h
)

add_executable(Norderhaug ${SOURCE_FILES})
