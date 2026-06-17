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
        ${CMAKE_SOURCE_DIR}/vendor/stb_image_write
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
        $<$<CONFIG:Debug>:-O2 -g -fno-omit-frame-pointer>
)

# ---- Engine resources ----------------------------------------------
set(SHADER_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SPRITES_DIR ${CMAKE_SOURCE_DIR}/sprites)
set(FONTS_DIR   ${CMAKE_SOURCE_DIR}/fonts)

file(GLOB SHADERS  CONFIGURE_DEPENDS "${SHADER_DIR}/*")
file(GLOB_RECURSE SPRITES CONFIGURE_DEPENDS "${SPRITES_DIR}/*")
file(GLOB FONTS CONFIGURE_DEPENDS "${FONTS_DIR}/*")

# Store separate resource types
set_property(TARGET GameEngine PROPERTY RESOURCE_FILES "${SHADERS};${FONTS}")
set_property(TARGET GameEngine PROPERTY SPRITE_FILES "${SPRITES}")

# Helper to copy resources to any executable
function(attach_resources target)
    # Copy shaders and fonts to root
    get_target_property(resources GameEngine RESOURCE_FILES)
    if (resources)
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory 
                    "$<TARGET_FILE_DIR:${target}>"
        )
        foreach(file IN LISTS resources)
            add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy 
                        "${file}"
                        "$<TARGET_FILE_DIR:${target}>/"
                COMMENT "Copying resource: ${file}"
            )
        endforeach()
    endif()

    # Copy sprites to sprites/ subdirectory
    get_target_property(sprites GameEngine SPRITE_FILES)
    if (sprites)
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory 
                    "$<TARGET_FILE_DIR:${target}>/sprites"
            COMMENT "Creating sprites directory"
        )
        foreach(file IN LISTS sprites)
            add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy 
                        "${file}"
                        "$<TARGET_FILE_DIR:${target}>/sprites/"
                COMMENT "Copying sprite: ${file}"
            )
        endforeach()
    endif()
endfunction()
