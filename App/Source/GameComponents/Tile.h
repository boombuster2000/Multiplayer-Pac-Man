#pragma once
#include "raylib.h"
#include <memory>

class Tile {

public:
    enum class Type
    {
        Wall,
        Path,
        Pellet,
        PowerPellet,
        Empty
    };

private:
    Type m_type;
    
    
public:
    Tile(Type type = Type::Empty);


    Type GetType() const;

    void Render(Vector2 position, Vector2 size) const;
};