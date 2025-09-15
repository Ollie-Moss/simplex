# Simplex

Simplex is a c++ library for creating high performant 2D games.

## Features & Roadmap
### MVP
- [x] Entity component system
- [x] Input handling and window management using `glfw`
- [x] 2D Rendering with `OpenGL`
- [x] Support 'modules' for different aspects of rendering, e.g. sprite renderer, line renderer, etc
- [ ] UI - *In progress*
- [ ] Audio via `OpenAL`
- [ ] Refactor asset management
- [ ] Scene management
- [ ] Basic components and systems e.g. physics, character controller, etc
- [ ] Test suite ```benchmarks, test cases, etc...```

### Stretch Goals
- [ ] Optimize ECS based on benchmarks and testing
- [ ] Lightweight editor for scene, and asset management
- [ ] Networking



## Installation

Clone this repository into project under a `third-party` directory and link it using cmake.
An example might look like this:
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyGame LANGUAGES CXX)

# Add Simplex
add_subdirectory(third-party/Simplex)

add_executable(MyGame src/main.cpp)

# Link Simplex
target_link_libraries(MyGame PRIVATE Simplex)

```

## Usage
See the `examples/` directory for more in depth and detailed examples of how games should be structured using this library. These examples are lightweight games meant to show of specific features of the library. 

```cpp
#include "core/Registry.h"
#include "core/Scene.h"
#include "core/Simplex.h"
#include "gui/UISystem.h"
#include "systems/CameraSystem.h"
#include "systems/RenderSystem.h"

int main()
{
    // Create simplex instance
    Simplex simplex;

    // Initialize
    if(!simplex.Init())
    {
        return 0;
    }

    // Setup a scene
    Scene MainScene = Scene("MainScene", [](Registry &m_Registry) {
        // Systems
        m_Registry.RegisterSystem<CameraSystem>();
        m_Registry.RegisterSystem<RenderSystem>();
        m_Registry.RegisterSystem<UISystem>();

        // Entities
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(-50, 100, 0)}, {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Camera>({}, {});
    });
    
    // Set scene
    simplex.SetScene(MainScene);
    
    // Start the game
    simplex.Start();
    return 0;
}

```

## Support
If you have issues or find a bug please create an issue with sufficient information including how to reproduce the bug, screenshots and steps along with development environment information.

## License
This project is licensed under the terms of the MIT license.



