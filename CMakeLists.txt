cmake_minimum_required(VERSION 3.10)

project(GameEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp" 
    "src/*.c" 
    "include/*.c"
    "include/*.cpp"
)

add_executable(GameEngine ${SOURCE_FILES})

target_include_directories(GameEngine PUBLIC 
    "include/"
    "src/"
    "third-party/"
)

add_compile_options(
    Wall
    Wextra
    g
)

target_link_libraries(GameEngine
    glfw
    freetype
    GL
    X11 Xrandr pthread dl Xi Xxf86vm Xinerama Xcursor
)

