#pragma once
#include "engine/serialization/json_converters.hpp"
#include "engine/serialization/json_helpers.hpp"
#include "engine/ui/grid.h"
#include "raylib.h"
#include <format>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using nlohmann::json;

namespace ui
{

// Template serialization for Grid<T>
template <typename T>
inline void to_json(json& j, const Grid<T>& grid)
{
    static_assert(std::is_base_of_v<GridTile, T>, "T must derive from ui::GridTile");

    // Serialize base RenderableObject
    to_json(j, static_cast<const RenderableObject&>(grid));

    j["tile_dimensions"] = grid.m_tileDimensions;
    j["spacing"] = grid.m_spacing;

    json gridArray = json::array();
    for (const auto& row : grid)
    {
        json jsonRow = json::array();
        for (const auto& tile : row)
        {
            json tile_j;
            to_json(tile_j, tile);
            jsonRow.push_back(tile_j);
        }
        gridArray.push_back(jsonRow);
    }

    j["grid"] = gridArray;
}

template <typename T>
inline void from_json(const json& j, Grid<T>& grid)
{
    static_assert(std::is_base_of_v<GridTile, T>, "T must derive from ui::GridTile");

    serialization::require_object(j, "Grid");

    // Deserialize base class
    from_json(j, static_cast<RenderableObject&>(grid));

    // ---- Required fields ----
    serialization::get_required_field(j, "tile_dimensions", grid.m_tileDimensions, "Grid", 500);
    serialization::get_required_field(j, "spacing", grid.m_spacing, "Grid", 501);

    // ---- Grid ----
    try
    {
        const auto& grid_j = j.at("grid");
        if (!grid_j.is_array())
            throw json::type_error::create(302, "Grid.grid must be an array of rows.", &grid_j);

        size_t y_size = grid_j.size();
        size_t x_size = 0;

        if (y_size > 0)
        {
            const auto& first_row = grid_j.at(0);

            if (!first_row.is_array())
                throw json::type_error::create(302, "Grid grid rows must be arrays.", &first_row);

            x_size = first_row.size();
        }

        grid.m_gridSize = {x_size, y_size};
        grid.m_grid.assign(y_size, std::vector<T>(x_size));

        for (size_t y = 0; y < y_size; ++y)
        {
            const auto& row_j = grid_j.at(y);
            if (!row_j.is_array())
                throw json::type_error::create(302, "Grid grid rows must be arrays.", &row_j);

            if (row_j.size() != x_size)
                throw json::type_error::create(
                    302, std::format("Grid grid row {} has {} elements, expected {}.", y, row_j.size(), x_size),
                    &row_j);

            for (size_t x = 0; x < x_size; ++x)
            {
                try
                {
                    row_j.at(x).get_to(grid.m_grid[y][x]);
                }
                catch (const json::exception& e)
                {
                    throw json::other_error::create(
                        502, std::format("Failed to deserialize Grid.grid[{}][{}]: {}", y, x, e.what()), &row_j);
                }
            }
        }
    }
    catch (const json::exception& e)
    {
        throw json::other_error::create(503, std::string("Failed to deserialize Grid.grid: ") + e.what(), &j);
    }
}
} // namespace ui