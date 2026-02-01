# tests/CMakeLists.txt

file(GLOB TEST_SOURCES "*.cpp")

add_executable(GameEngineTests
    ${TEST_SOURCES}
)

target_link_libraries(GameEngineTests
    PRIVATE
        GameEngine
        Catch2::Catch2WithMain
)

# Discover Catch2 tests automatically
include(Catch)
catch_discover_tests(GameEngineTests)
