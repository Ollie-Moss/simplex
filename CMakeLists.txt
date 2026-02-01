cmake_minimum_required(VERSION 3.10)

project(GameEngine
    VERSION 0.1
    LANGUAGES C CXX
)

# ---- Global config -------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type (nice for single-config generators)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# ---- Third-party dependencies -------------------------------------
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/freetype)
add_subdirectory(vendor/catch2)

# glad (no official CMake target)
add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# ---- Engine --------------------------------------------------------
add_subdirectory(src)

# ---- Examples ------------------------------------------------------
option(BUILD_EXAMPLES "Build example programs" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ---- Tests ---------------------------------------------------------
option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
