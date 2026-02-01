# examples/CMakeLists.txt

file(GLOB EXAMPLE_SOURCES "*.cpp")

foreach(example ${EXAMPLE_SOURCES})
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example})

    target_link_libraries(${name}
        PRIVATE
            GameEngine
    )

    # Copy engine resources (shaders, sprites, fonts)
    attach_resources(${name})
endforeach()
