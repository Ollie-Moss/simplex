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


add_library(GameEngine ${SOURCE_FILES})
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

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
set(SPRITES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sprites)
set(FONTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fonts)

file(GLOB SHADERS ${SHADER_DIR}/*)
file(GLOB SPRITES ${SPRITES_DIR}/*)
file(GLOB FONTS ${FONTS_DIR}/*)
set(ALL_RESOURCES ${SHADERS} ${SPRITES} ${FONTS})

set_property(TARGET GameEngine PROPERTY RESOURCE_FILES "${ALL_RESOURCES}")

function(attach_resources target)
    get_target_property(resources GameEngine RESOURCE_FILES)
    if(resources)
        foreach(file IN LISTS resources)
            add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${file}
                        $<TARGET_FILE_DIR:${target}>
            )
        endforeach()
    endif()
endfunction()


add_executable(build "examples/main.cpp")
target_link_libraries(build GameEngine)

# Apply to this exe
attach_resources(build)
