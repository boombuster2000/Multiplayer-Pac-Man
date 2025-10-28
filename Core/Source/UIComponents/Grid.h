#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include "UIComponents/RenderableObject.h"
#include "UIComponents/GridTile.h"
#include "UIComponents/Enums.h"
#include "DataTypes/Vector2Ex.h"

namespace UIComponents
{
    template <class T = GridTile>
    class Grid : public RenderableObject
    {
        static_assert(std::is_base_of<GridTile, T>::value,
                      "T must derive from UIComponents::GridTile");

    private:
        using GridType = std::vector<std::vector<T>>;
        Vector2Ex<int> m_tileDimensions;

    protected:
        GridType m_grid;

    public:
        template <typename... Args>
        Grid(Vector2Ex<size_t> arraySize,
             Vector2Ex<int> tileDimensions,
             Vector2Ex<int> anchorPointPosition,
             UIComponents::AnchorPoint anchorPoint,
             Vector2Ex<int> spacing,
             Args &&...tileArgs)
            : RenderableObject(anchorPointPosition, anchorPoint),
              m_tileDimensions(tileDimensions)
        {

            m_grid.reserve(arraySize.y);

            for (int y = 0; y < arraySize.y; y++)
            {
                std::vector<T> row;
                row.reserve(arraySize.x);

                for (int x = 0; x < arraySize.x; x++)
                {
                    row.emplace_back(std::forward<Args>(tileArgs)...);
                    row[x].SetAnchorPointPosition(Vector2Ex<int>(anchorPointPosition.x + ((x * tileDimensions.x) + spacing.x),
                                                                 anchorPointPosition.y + (y * tileDimensions.y) + spacing.y));
                }

                m_grid.emplace_back(std::move(row));
            }

            UpdateDrawPoint();
        }

        ~Grid() = default;

        std::vector<T> &operator[](size_t y) { return m_grid[y]; }
        const std::vector<T> &operator[](size_t y) const { return m_grid[y]; }

        auto begin() { return m_grid.begin(); }
        auto end() { return m_grid.end(); }
        auto begin() const { return m_grid.begin(); }
        auto end() const { return m_grid.end(); }

        virtual void Set(int y, int x, T object)
        {
            m_grid[y][x] = object;
        }

        // Rendering and size
        Vector2Ex<int> GetDimensions() const override
        {
            if (m_grid.empty() || m_grid.front().empty())
                return {0, 0};

            int width = m_grid.front().size() * m_grid.front().front().GetDimensions().x;
            int height = m_grid.size() * m_grid.front().front().GetDimensions().y;
            return {width, height};
        }

        Vector2Ex<int> GetTileDimensions() const
        {
            return m_tileDimensions;
        }

        Vector2Ex<int> GetRelativeIndexFromPosition(const Vector2Ex<int> &position) const
        {
            using namespace UIComponents;

            const float boardLeftPos = GetPosition().x;
            const float boardRightPos = GetPosition(AnchorPoint::TOP_RIGHT).x;
            const float boardTopPos = GetPosition().y;
            const float boardBottomPos = GetPosition(AnchorPoint::BOTTOM_RIGHT).y;

            const float width = boardRightPos - boardLeftPos;
            const float height = boardBottomPos - boardTopPos;

            if (width == 0.0f || height == 0.0f)
                return {0, 0}; // Prevent divide by zero

            const float relativeXPos = (position.x - boardLeftPos) / width;
            const float relativeYPos = (position.y - boardTopPos) / height;

            const int cols = static_cast<int>(m_grid[0].size());
            const int rows = static_cast<int>(m_grid.size());

            const int relativeXIndex = std::clamp(static_cast<int>(cols * relativeXPos), 0, cols - 1);
            const int relativeYIndex = std::clamp(static_cast<int>(rows * relativeYPos), 0, rows - 1);

            return Vector2Ex<int>(relativeXIndex, relativeYIndex);
        }

        Vector2Ex<int> GetPositionFromIndex(const Vector2Ex<int> &index) const
        {
            if (!IsValidIndex(index))
                throw std::out_of_range("Not valid index.");

            return m_grid[index.y][index.x].GetPosition();
        }

        // Accessors
        T &GetTile(const Vector2Ex<int> &index)
        {
            return m_grid[index.y][index.x];
        }

        const T &GetTile(const Vector2Ex<int> &index) const
        {
            return m_grid[index.y][index.x];
        }

        T &GetTileFromPosition(const Vector2Ex<int> &position)
        {
            const Vector2Ex<int> index = GetRelativeIndexFromPosition(position);
            return m_grid[index.y][index.x];
        }

        const T &GetTileFromPosition(const Vector2Ex<int> &position) const
        {
            const Vector2Ex<int> index = GetRelativeIndexFromPosition(position);
            return m_grid[index.y][index.x];
        }

        // Other
        bool IsValidIndex(const Vector2Ex<size_t> &index) const
        {
            if (index.y < 0)
                return false;
            if (index.x < 0)
                return false;

            if (index.y >= m_grid.size())
                return false;
            if (index.x >= m_grid.front().size())
                return false;

            return true;
        }

        bool IsValidIndex(const Vector2Ex<int> &index) const
        {
            if (index.y < 0)
                return false;
            if (index.x < 0)
                return false;

            if (index.y >= m_grid.size())
                return false;
            if (index.x >= m_grid.front().size())
                return false;

            return true;
        }

        virtual void Render(Vector2Ex<int> offset = {0, 0}) const override
        {
            for (const auto &row : m_grid)
                for (const auto &tile : row)
                    tile.Render();
        }
    };
}
