cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 20)

project(producer_consumer)
add_executable(${PROJECT_NAME} "main.cpp" "one_shot_event.cpp")
