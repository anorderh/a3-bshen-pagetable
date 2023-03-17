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
)

add_executable(Norderhaug ${SOURCE_FILES})
