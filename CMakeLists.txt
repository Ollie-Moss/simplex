#cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)

project(GameEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory( vendor/glfw )
add_subdirectory( vendor/glm )
add_subdirectory( vendor/freetype )

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp" 
    "src/*.c" 
)
add_executable(GameEngine ${SOURCE_FILES})
target_include_directories(GameEngine PUBLIC 
    "src/"
    "vendor/stb_image"
)


target_link_libraries(GameEngine
    glad
    glm
    glfw
    freetype
    GL
    X11 
    Xrandr 
    pthread 
    dl 
    Xi 
    Xxf86vm 
    Xinerama 
    Xcursor 
    g 
    -O0
)

