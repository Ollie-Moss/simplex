#cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)

project(GameEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
include(FetchContent)

# ---- GLFW ----
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        latest
)
FetchContent_MakeAvailable(glfw)

# ---- Freetype ----
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG        VER-2-13-0
)
FetchContent_MakeAvailable(freetype)

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

target_link_libraries(GameEngine
    glfw
    freetype
    GL
    X11 Xrandr pthread dl Xi Xxf86vm Xinerama Xcursor g -O0
)

