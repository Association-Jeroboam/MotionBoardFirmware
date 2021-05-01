cmake_minimum_required(VERSION 3.19)

project(test_strategy)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} 
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../common)
target_include_directories(${PROJECT_NAME} PRIVATE ../libs/eHSM/include)
target_include_directories(${PROJECT_NAME} PRIVATE ./)
