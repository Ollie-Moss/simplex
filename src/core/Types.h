#pragma once

#include <bitset>
#include <cstdint>
#include <glm/glm.hpp>
#include "core/View.h"

const uint32_t MAX_BUFFER_SIZE = 1000;
const uint32_t MAX_RENDER_BUFFERS = 100;

enum class ProjectionType { ScreenSpace, WorldSpace };

using TextureName = std::string;

const uint32_t MAX_ENTITIES = 100000;  // 100,000
const uint32_t MAX_COMPONENTS = 100;

using ComponentId = uint32_t;
using EntityId = uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;

const EntityId NULL_ENTITY = MAX_ENTITIES + 1;

using Padding = RectBounds<float>;
using Color = glm::vec4;

// textures
const std::string NO_TEXTURE = "";

// Basic colors
const Color WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color RED = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color YELLOW = Color(1.0f, 1.0f, 0.0f, 1.0f);
const Color CYAN = Color(0.0f, 1.0f, 1.0f, 1.0f);
const Color MAGENTA = Color(1.0f, 0.0f, 1.0f, 1.0f);
const Color GRAY = Color(0.5f, 0.5f, 0.5f, 1.0f);
const Color OPAQUE = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color TRANSPARENT = Color(0.0f, 0.0f, 0.0f, 0.0f);
