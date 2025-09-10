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

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(GLOB SHADERS ${SHADER_DIR}/*)

add_library(GameEngine ${SOURCE_FILES})
target_include_directories(GameEngine PUBLIC 
    "src/"
    "vendor/stb_image"
)
target_compile_definitions(GameEngine
    PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
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

# Copy shaders to the binary dir (so relative paths work)
foreach(shader ${SHADERS})
    add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${shader}
            $<TARGET_FILE_DIR:GameEngine>
    )
endforeach()
