#pragma once

#include <bitset>
#include <cstdint>

const std::uint32_t MAX_ENTITIES = 100000; // 100,000
const std::uint32_t MAX_COMPONENTS = 100;

using ComponentId = std::uint32_t;
using EntityId = std::uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;

