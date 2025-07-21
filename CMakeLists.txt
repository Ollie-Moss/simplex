cmake_minimum_required(VERSION 3.10)

project(GameEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.c")

add_executable(GameEngine ${SOURCE_FILES})

target_include_directories(GameEngine PUBLIC 
    "include/"
    "src/"
    )

