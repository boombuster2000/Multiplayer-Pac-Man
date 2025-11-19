#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

#include "engine/serialization/renderable_object_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/enums.h"
#include "engine/ui/grid_tile.h"

namespace ui
{
template <class T = GridTile> class Grid : public RenderableObject
{
    static_assert(std::is_base_of<GridTile, T>::value, "T must derive from ui::GridTile");

  private:
    using GridType = std::vector<std::vector<T>>;
    Vector2Ex<float> m_tileDimensions;
    Vector2Ex<float> m_spacing;
    Vector2Ex<size_t> m_gridSize;

    GridType m_grid;

  public:
    template <typename... Args>
    Grid()
        : RenderableObject(), m_tileDimensions(Vector2Ex<float>(0, 0)), m_spacing(Vector2Ex<float>(0, 0)),
          m_gridSize(Vector2Ex<size_t>(0, 0)), m_grid(GridType())
    {
    }

    template <typename... Args>
    Grid(Vector2Ex<size_t> arraySize, Vector2Ex<float> tileDimensions, Vector2Ex<float> anchorPointPosition,
         ui::AnchorPoint anchorPoint, Vector2Ex<float> spacing, Args&&... tileArgs)
        : RenderableObject(anchorPointPosition, anchorPoint), m_tileDimensions(tileDimensions), m_spacing(spacing),
          m_gridSize(arraySize)
    {
        // Calculate grid dimensions
        float width = arraySize.x * tileDimensions.x + (arraySize.x > 0 ? (arraySize.x - 1) * spacing.x : 0);
        float height = arraySize.y * tileDimensions.y + (arraySize.y > 0 ? (arraySize.y - 1) * spacing.y : 0);

        // Calculate anchor point offset
        Vector2Ex<float> anchorOffset = RenderableObject::CalculateAnchorOffset(anchorPoint, width, height);

        m_grid.reserve(arraySize.y);

        for (int y = 0; y < arraySize.y; y++)
        {
            std::vector<T> row;
            row.reserve(arraySize.x);

            for (int x = 0; x < arraySize.x; x++)
            {
                row.emplace_back(std::forward<Args>(tileArgs)...);
                row[x].SetPosition(Vector2Ex<float>(
                    anchorPointPosition.x + -anchorOffset.x + (x * tileDimensions.x) + (x * spacing.x),
                    anchorPointPosition.y + -anchorOffset.y + (y * tileDimensions.y) + (y * spacing.y)));
            }

            m_grid.emplace_back(std::move(row));
        }
        SetOrigin(anchorPoint);
    }

    ~Grid() = default;

    std::vector<T>& operator[](size_t y)
    {
        return m_grid[y];
    }
    const std::vector<T>& operator[](size_t y) const
    {
        return m_grid[y];
    }

    auto begin()
    {
        return m_grid.begin();
    }
    auto end()
    {
        return m_grid.end();
    }
    auto begin() const
    {
        return m_grid.begin();
    }
    auto end() const
    {
        return m_grid.end();
    }

    virtual void Set(int y, int x, T object)
    {
        m_grid[y][x] = object;
    }

    // Getters and Setters
    Vector2Ex<float> GetDimensions() const override
    {
        if (m_grid.empty() || m_grid.front().empty())
            return {0, 0};

        size_t num_cols = m_grid.front().size();
        size_t num_rows = m_grid.size();

        float width = num_cols * m_tileDimensions.x + (num_cols > 0 ? (num_cols - 1) * m_spacing.x : 0);
        float height = num_rows * m_tileDimensions.y + (num_rows > 0 ? (num_rows - 1) * m_spacing.y : 0);
        return {width, height};
    }

    Vector2Ex<float> GetTileDimensions() const
    {
        return m_tileDimensions;
    }

    void SetTileDimensions(const Vector2Ex<float>& dimensions)
    {
        m_tileDimensions = dimensions;
    }

    Vector2Ex<float> GetSpacing() const
    {
        return m_spacing;
    }

    void SetSpacing(const Vector2Ex<float>& spacing)
    {
        m_spacing = spacing;
    }

    Vector2Ex<size_t> GetGridSize() const
    {
        return m_gridSize;
    }

    void SetGridSize(const Vector2Ex<size_t>& gridSize)
    {
        m_gridSize = gridSize;

        m_grid.resize(gridSize.y);
        for (auto& row : m_grid)
        {
            row.resize(gridSize.x);
        }
    }

    Vector2Ex<int> GetRelativeIndexFromPosition(const Vector2Ex<float>& position) const
    {
        Vector2Ex<float> topLeft = GetPositionAtAnchor();
        Vector2Ex<float> relativePos = position - topLeft;

        if (m_tileDimensions.x == 0 || m_tileDimensions.y == 0)
            return {0, 0};

        int x_index = relativePos.x / (m_tileDimensions.x + m_spacing.x);
        int y_index = relativePos.y / (m_tileDimensions.y + m_spacing.y);

        int num_cols = m_grid.empty() ? 0 : m_grid.front().size();
        int num_rows = m_grid.size();

        x_index = std::clamp(x_index, 0, num_cols > 0 ? num_cols - 1 : 0);
        y_index = std::clamp(y_index, 0, num_rows > 0 ? num_rows - 1 : 0);

        return {x_index, y_index};
    }

    Vector2Ex<float> GetPositionFromIndex(const Vector2Ex<int>& index) const
    {
        if (!IsValidIndex(index))
            throw std::out_of_range("Not valid index.");

        return m_grid[index.y][index.x].GetPositionAtAnchor();
    }

    // Accessors
    T& GetTile(const int y, const int x)
    {
        return m_grid[y][x];
    }

    const T& GetTile(const int y, const int x) const
    {
        return m_grid[y][x];
    }

    T& GetTile(const Vector2Ex<int>& index)
    {
        return m_grid[index.y][index.x];
    }

    const T& GetTile(const Vector2Ex<int>& index) const
    {
        return m_grid[index.y][index.x];
    }

    T& GetTileFromPosition(const Vector2Ex<float>& position)
    {
        const Vector2Ex<int> index = GetRelativeIndexFromPosition(position);
        return m_grid[index.y][index.x];
    }

    const T& GetTileFromPosition(const Vector2Ex<float>& position) const
    {
        const Vector2Ex<int> index = GetRelativeIndexFromPosition(position);
        return m_grid[index.y][index.x];
    }

    // Other
    bool IsValidIndex(const Vector2Ex<size_t>& index) const
    {
        if (m_grid.empty() || index.y >= m_grid.size())
            return false;
        if (m_grid.front().empty() || index.x >= m_grid.front().size())
            return false;

        return true;
    }

    bool IsValidIndex(const Vector2Ex<int>& index) const
    {
        if (index.y < 0 || index.x < 0)
            return false;

        if (m_grid.empty() || (size_t)index.y >= m_grid.size())
            return false;
        if (m_grid.front().empty() || (size_t)index.x >= m_grid.front().size())
            return false;

        return true;
    }

    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override
    {
        for (const auto& row : m_grid)
            for (const auto& tile : row)
                tile.Render(offset);
    }
};
} // namespace ui
