cmake_minimum_required(VERSION 3.10.3)

#project name
project(Network)

#add files to compile
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/Backend/*.cpp)
file(GLOB SERVER ${PROJECT_SOURCE_DIR}/src/Server/server.cpp)
file(GLOB CLIENT ${PROJECT_SOURCE_DIR}/src/Client/client.cpp)
add_executable(Server ${SERVER} ${SOURCES})
add_executable(Client ${CLIENT} ${SOURCES})

#export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Compiler Flags
set(CMAKE_CXX_FLAGS "-std=c++1z -W -Wall -Wextra -Werror -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
#-lws2_32 if windows

#Paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#Include
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/Backend")
include_directories("${PROJECT_SOURCE_DIR}/include")

#FMT
add_subdirectory("${PROJECT_SOURCE_DIR}/../../ext/fmt" EXCLUDE_FROM_ALL "${PROJECT_SOURCE_DIR}/build/third_party/fmt")
target_link_libraries(Server fmt::fmt-header-only)
target_link_libraries(Client fmt::fmt-header-only)
