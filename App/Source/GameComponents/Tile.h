#pragma once
#include <memory>
#include "raylib.h"
#include "UIComponents/GridTile.h"
#include "DataTypes/Vector2Ex.h"

class Tile : public UIComponents::GridTile
{

public:
    enum class Type : int
    {
        Wall,
        Path,
        None,
    };


private:
    Type m_type;

public:
    Tile();
    Tile(Type type, Vector2Ex<float> postion, Vector2Ex<float> dimensions);

    Type GetType() const;
    void SetType(Type type);

    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
};