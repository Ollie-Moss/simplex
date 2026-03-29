# Collect engine sources
file(GLOB_RECURSE GAMEENGINE_SOURCES
    "*.cpp"
    "*.c"
)

add_library(GameEngine ${GAMEENGINE_SOURCES})

# Public headers = usable by examples/tests
target_include_directories(GameEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/vendor/stb_image
)

# Engine dependencies
target_link_libraries(GameEngine
    PUBLIC
        glad
        glm
        glfw
        freetype
    PRIVATE
        GL
        X11
        Xrandr
        pthread
        dl
        Xi
        Xxf86vm
        Xinerama
        Xcursor
)

# ---- Debug options -------------------------------------------------
target_compile_options(GameEngine
    PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
)

target_link_options(GameEngine
    PRIVATE
        $<$<CONFIG:Debug>:-pg>
)

# ---- Engine resources ----------------------------------------------
set(SHADER_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SPRITES_DIR ${CMAKE_SOURCE_DIR}/sprites)
set(FONTS_DIR   ${CMAKE_SOURCE_DIR}/fonts)

file(GLOB SHADERS  CONFIGURE_DEPENDS "${SHADER_DIR}/*")
file(GLOB SPRITES CONFIGURE_DEPENDS "${SPRITES_DIR}/*")
file(GLOB FONTS CONFIGURE_DEPENDS "${FONTS_DIR}/*")

set(ALL_RESOURCES ${SHADERS} ${SPRITES} ${FONTS})

set_property(TARGET GameEngine PROPERTY RESOURCE_FILES "${ALL_RESOURCES}")

# Helper to copy resources to any executable
function(attach_resources target)
    get_target_property(resources GameEngine RESOURCE_FILES)
    if (resources)
        foreach(file IN LISTS resources)
            add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy 
                        ${file}
                        $<TARGET_FILE_DIR:${target}>
            )
        endforeach()
    endif()
endfunction()
