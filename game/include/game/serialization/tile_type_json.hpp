#pragma once
#include "game/components/tile.h"
#include <nlohmann/json.hpp>

NLOHMANN_JSON_SERIALIZE_ENUM(Tile::Type, {
                                             {Tile::Type::WALL, "WALL"},
                                             {Tile::Type::PATH, "PATH"},
                                             {Tile::Type::NONE, "None"},
                                         })