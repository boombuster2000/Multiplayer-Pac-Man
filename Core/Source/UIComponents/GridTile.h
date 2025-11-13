#pragma once
#include "raylib.h"
#include "UIComponents/RenderableObject.h"
#include "DataTypes/Vector2Ex.h"

namespace UIComponents
{
    class GridTile : public RenderableObject
    {

    private:
        Vector2Ex<float> m_dimensions;

    public:
        GridTile();
        GridTile(Vector2Ex<float> dimensions, Vector2Ex<float> position);
        ~GridTile() = default;

        virtual void SetDimensions(const Vector2Ex<float>& dimensions);
        virtual Vector2Ex<float> GetDimensions() const override;

        virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
    };
}
