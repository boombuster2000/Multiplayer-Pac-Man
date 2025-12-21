#pragma once

#include "engine/core/vector2ex.h"
#include "engine/ui/renderable_object.h"
#include "raylib.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

class Pellet : public ui::RenderableObject
{
  public:
    enum class Type : int
    {
        // Point values
        NORMAL,
        SUPER,
        NONE
    };

    struct TypePointValues
    {
        int normal;
        int super;
        int none;
    };

  private:
    Type m_type;
    float radius = 3.0f;
    bool m_isEaten = false;

    static inline TypePointValues s_typePoints{
        10, // Normal
        50, // Super
        0   // NONE
    };

  public:
    Pellet();
    Pellet(Vector2Ex<float> position, Type type);

    Type GetType() const;
    void SetType(const Type& type);

    int GetValue() const;

    bool IsEaten() const;
    void SetIsEaten(bool isEaten);

    static int GetTypePoints(const Type& t);
    static void SetTypePoints(const TypePointValues& newValues);

    Vector2Ex<float> GetDimensions() const override;
    void Render(Vector2Ex<float> offset = {0, 0}) const override;

    friend void to_json(json&, const Pellet&);
    friend void from_json(const json&, Pellet&);
};