#pragma once
#include "engine/ui/enums.h"
#include <nlohmann/json.hpp>

// Using NLOHMANN_JSON_SERIALIZE_ENUM is more concise and less error-prone.
// It throws nlohmann::json::out_of_range for invalid values during deserialization,
// which is consistent with the library's behavior for other types.
namespace ui
{

NLOHMANN_JSON_SERIALIZE_ENUM(AnchorPoint,
                             {{AnchorPoint::TOP_LEFT, "TOP_LEFT"},
                              {AnchorPoint::TOP_MIDDLE, "TOP_MIDDLE"},
                              {AnchorPoint::TOP_RIGHT, "TOP_RIGHT"},
                              {AnchorPoint::MIDDLE_LEFT, "MIDDLE_LEFT"},
                              {AnchorPoint::MIDDLE, "MIDDLE"},
                              {AnchorPoint::MIDDLE_RIGHT, "MIDDLE_RIGHT"},
                              {AnchorPoint::BOTTOM_LEFT, "BOTTOM_LEFT"},
                              {AnchorPoint::BOTTOM_MIDDLE, "BOTTOM_MIDDLE"},
                              {AnchorPoint::BOTTOM_RIGHT, "BOTTOM_RIGHT"}})

NLOHMANN_JSON_SERIALIZE_ENUM(Alignment, {{Alignment::LEFT, "LEFT"}, {Alignment::CENTER, "CENTER"}, {Alignment::RIGHT, "RIGHT"}})

NLOHMANN_JSON_SERIALIZE_ENUM(Direction,
                             {{Direction::UP, "UP"},
                              {Direction::DOWN, "DOWN"},
                              {Direction::LEFT, "LEFT"},
                              {Direction::RIGHT, "RIGHT"}})

} // namespace ui