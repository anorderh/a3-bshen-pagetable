cmake_minimum_required(VERSION 3.21)
project(Norderhaug)

set(CMAKE_CXX_STANDARD 11)

set(APP_DOMAIN ./src)

include_directories(${APP_DOMAIN})

set(
    SOURCE_FILES
        ${APP_DOMAIN}/print_helpers.h
        ${APP_DOMAIN}/print_helpers.c
        ${APP_DOMAIN}/vaddr_tracereader.h
        ${APP_DOMAIN}/vaddr_tracereader.c
        ${APP_DOMAIN}/main.cpp
        ${APP_DOMAIN}/bitManipulation.cpp
        ${APP_DOMAIN}/Map.cpp
        ${APP_DOMAIN}/Map.h
        ${APP_DOMAIN}/PageTable.cpp
        ${APP_DOMAIN}/PageTable.h
        ${APP_DOMAIN}/PageTableStats.cpp
        ${APP_DOMAIN}/PageTableStats.h
        ${APP_DOMAIN}/Level.cpp
        ${APP_DOMAIN}/Level.h
        ${APP_DOMAIN}/TLB.cpp
        ${APP_DOMAIN}/TLB.h
        ${APP_DOMAIN}/ArgParser.cpp
        ${APP_DOMAIN}/ArgParser.h
)

add_executable(Norderhaug ${SOURCE_FILES})
