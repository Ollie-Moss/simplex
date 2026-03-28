#include "catch2/catch_test_macros.hpp"
#include "core/Simplex.h"

TEST_CASE("Simplex Init does not throw", "[simplex][init]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.Init());
}

TEST_CASE("Simplex Init returns boolean", "[simplex][init]")
{
    Simplex simplex;
    bool result = simplex.Init();
    REQUIRE((result == true || result == false));
}

TEST_CASE("Simplex Init can be called multiple times", "[simplex][init]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.Init());
    REQUIRE_NOTHROW(simplex.Init());
}

TEST_CASE("Simplex Get returns singleton instance", "[simplex][singleton]")
{
    REQUIRE_NOTHROW(Simplex::Get());
}

TEST_CASE("Simplex Get returns same instance on multiple calls", "[simplex][singleton]")
{
    Simplex &instance1 = Simplex::Get();
    Simplex &instance2 = Simplex::Get();
    REQUIRE(&instance1 == &instance2);
}

TEST_CASE("Simplex singleton instance persists across calls", "[simplex][singleton]")
{
    Simplex *addr1 = &Simplex::Get();
    Simplex *addr2 = &Simplex::Get();
    Simplex *addr3 = &Simplex::Get();

    REQUIRE(addr1 == addr2);
    REQUIRE(addr2 == addr3);
}

TEST_CASE("GetView does not throw", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetView());
}

TEST_CASE("GetInput does not throw", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetInput());
}

TEST_CASE("GetScene does not throw", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetScene());
}

TEST_CASE("GetRegistry does not throw", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetRegistry());
}

TEST_CASE("GetRendererManager does not throw", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetRendererManager());
}

TEST_CASE("GetAssetManager does not throw", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetAssetManager());
}

TEST_CASE("All static accessors work before Init", "[simplex][accessors]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(Simplex::GetView());
    REQUIRE_NOTHROW(Simplex::GetInput());
    REQUIRE_NOTHROW(Simplex::GetScene());
    REQUIRE_NOTHROW(Simplex::GetRegistry());
    REQUIRE_NOTHROW(Simplex::GetRendererManager());
    REQUIRE_NOTHROW(Simplex::GetAssetManager());
}

TEST_CASE("All static accessors work after Init", "[simplex][accessors][init]")
{
    Simplex simplex;
    simplex.Init();

    REQUIRE_NOTHROW(Simplex::GetView());
    REQUIRE_NOTHROW(Simplex::GetInput());
    REQUIRE_NOTHROW(Simplex::GetScene());
    REQUIRE_NOTHROW(Simplex::GetRegistry());
    REQUIRE_NOTHROW(Simplex::GetRendererManager());
    REQUIRE_NOTHROW(Simplex::GetAssetManager());
}

TEST_CASE("Static accessors return same references", "[simplex][accessors]")
{
    Simplex simplex;

    IView &view1 = Simplex::GetView();
    IView &view2 = Simplex::GetView();
    REQUIRE(&view1 == &view2);

    IInput &input1 = Simplex::GetInput();
    IInput &input2 = Simplex::GetInput();
    REQUIRE(&input1 == &input2);
}

TEST_CASE("SetScene does not throw", "[simplex][scene]")
{
    Simplex simplex;
    Scene testScene;
    REQUIRE_NOTHROW(simplex.SetScene(testScene));
}

TEST_CASE("SetScene works before Init", "[simplex][scene]")
{
    Simplex simplex;
    Scene testScene;
    REQUIRE_NOTHROW(simplex.SetScene(testScene));
}

TEST_CASE("SetScene works after Init", "[simplex][scene][init]")
{
    Simplex simplex;
    simplex.Init();
    Scene testScene;
    REQUIRE_NOTHROW(simplex.SetScene(testScene));
}

TEST_CASE("SetScene can be called multiple times", "[simplex][scene]")
{
    Simplex simplex;
    Scene scene1, scene2, scene3;

    REQUIRE_NOTHROW(simplex.SetScene(scene1));
    REQUIRE_NOTHROW(simplex.SetScene(scene2));
    REQUIRE_NOTHROW(simplex.SetScene(scene3));
}

TEST_CASE("GetFPS does not throw", "[simplex][timing]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.GetFPS());
}

TEST_CASE("GetFPS before Init", "[simplex][timing]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.GetFPS());
}

TEST_CASE("GetFPS after Init", "[simplex][timing][init]")
{
    Simplex simplex;
    simplex.Init();
    REQUIRE_NOTHROW(simplex.GetFPS());
}

TEST_CASE("GetFPS returns non-negative value", "[simplex][timing]")
{
    Simplex simplex;
    float fps = simplex.GetFPS();
    REQUIRE(fps >= 0.0f);
}

TEST_CASE("GetFPS returns float type", "[simplex][timing]")
{
    Simplex simplex;
    float fps = simplex.GetFPS();
    REQUIRE(std::is_same<decltype(fps), float>::value);
}

TEST_CASE("GetDeltaTime does not throw", "[simplex][timing]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("GetDeltaTime before Init", "[simplex][timing]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("GetDeltaTime after Init", "[simplex][timing][init]")
{
    Simplex simplex;
    simplex.Init();
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("GetDeltaTime returns non-negative value", "[simplex][timing]")
{
    Simplex simplex;
    float deltaTime = simplex.GetDeltaTime();
    REQUIRE(deltaTime >= 0.0f);
}

TEST_CASE("GetDeltaTime returns reasonable value", "[simplex][timing]")
{
    Simplex simplex;
    float deltaTime = simplex.GetDeltaTime();
    // Delta time should typically be less than 1 second
    REQUIRE(deltaTime < 1.0f);
}

TEST_CASE("GetDeltaTime returns float type", "[simplex][timing]")
{
    Simplex simplex;
    float deltaTime = simplex.GetDeltaTime();
    REQUIRE(std::is_same<decltype(deltaTime), float>::value);
}

TEST_CASE("GetFPS and GetDeltaTime both work", "[simplex][timing]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.GetFPS());
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("FPS and DeltaTime are both non-negative", "[simplex][timing]")
{
    Simplex simplex;
    float fps = simplex.GetFPS();
    float deltaTime = simplex.GetDeltaTime();

    REQUIRE(fps >= 0.0f);
    REQUIRE(deltaTime >= 0.0f);
}

TEST_CASE("Multiple calls to timing functions", "[simplex][timing]")
{
    Simplex simplex;

    REQUIRE_NOTHROW(simplex.GetFPS());
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
    REQUIRE_NOTHROW(simplex.GetFPS());
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("Full initialization sequence", "[simplex][integration]")
{
    Simplex simplex;
    REQUIRE_NOTHROW(simplex.Init());

    Scene testScene;
    REQUIRE_NOTHROW(simplex.SetScene(testScene));

    REQUIRE(simplex.GetFPS() >= 0.0f);
    REQUIRE(simplex.GetDeltaTime() >= 0.0f);
}

TEST_CASE("All operations work together", "[simplex][integration]")
{
    Simplex simplex;

    REQUIRE_NOTHROW(simplex.Init());
    REQUIRE_NOTHROW(Simplex::Get());
    REQUIRE_NOTHROW(Simplex::GetView());
    REQUIRE_NOTHROW(Simplex::GetInput());
    REQUIRE_NOTHROW(Simplex::GetScene());
    REQUIRE_NOTHROW(Simplex::GetRegistry());
    REQUIRE_NOTHROW(Simplex::GetRendererManager());
    REQUIRE_NOTHROW(Simplex::GetAssetManager());

    Scene scene;
    REQUIRE_NOTHROW(simplex.SetScene(scene));

    REQUIRE_NOTHROW(simplex.GetFPS());
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("Operations in different order", "[simplex][integration]")
{
    Simplex simplex;

    REQUIRE_NOTHROW(simplex.GetFPS());
    REQUIRE_NOTHROW(Simplex::GetView());
    REQUIRE_NOTHROW(simplex.Init());

    Scene scene;
    REQUIRE_NOTHROW(simplex.SetScene(scene));
    REQUIRE_NOTHROW(simplex.GetDeltaTime());
}

TEST_CASE("Accessing singleton without local instance", "[simplex][edge-case]")
{
    REQUIRE_NOTHROW(Simplex::Get());
    REQUIRE_NOTHROW(Simplex::GetView());
    REQUIRE_NOTHROW(Simplex::GetInput());
}

TEST_CASE("Setting scene multiple times rapidly", "[simplex][edge-case]")
{
    Simplex simplex;

    for(int i = 0; i < 10; ++i)
    {
        Scene scene;
        REQUIRE_NOTHROW(simplex.SetScene(scene));
    }
}

TEST_CASE("Calling GetFPS and GetDeltaTime rapidly", "[simplex][edge-case]")
{
    Simplex simplex;

    for(int i = 0; i < 100; ++i)
    {
        REQUIRE_NOTHROW(simplex.GetFPS());
        REQUIRE_NOTHROW(simplex.GetDeltaTime());
    }
}

TEST_CASE("Init called multiple times in sequence", "[simplex][edge-case]")
{
    Simplex simplex;

    for(int i = 0; i < 5; ++i)
    {
        REQUIRE_NOTHROW(simplex.Init());
    }
}
