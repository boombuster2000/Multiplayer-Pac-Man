#pragma once
#include "raylib.h"
#include "UIComponents/RenderableObject.h"
#include "DataTypes/Vector2Ex.h"

namespace UIComponents
{
    class GridTile : public RenderableObject
    {

    private:
        Vector2Ex<int> m_dimensions;

    public:
        GridTile(Vector2Ex<int> dimensions, Vector2Ex<int> position);
        ~GridTile() = default;

        virtual Vector2Ex<int> GetDimensions() const override;
        virtual void Render(Vector2Ex<int> offset = {0, 0}) const override;
    };
}
