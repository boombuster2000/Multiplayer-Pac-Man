#include "game/components/pellet.h"
#include "game/game_application.h"

Pellet::Pellet() :
    RenderableObject(),
    m_type(Type::NONE)
{
}

Pellet::Pellet(Vector2Ex<float> position, const Type type) :
    RenderableObject(position, ui::AnchorPoint::MIDDLE),
    m_type(type)
{
}

Pellet::Type Pellet::GetType() const
{
    return m_type;
}

void Pellet::SetType(const Type& type)
{
    m_type = type;
}

int Pellet::GetValue() const
{
    return Pellet::GetTypePoints(m_type);
}

bool Pellet::IsEaten() const
{
    return m_isEaten;
}

void Pellet::SetIsEaten(const bool isEaten)
{
    m_isEaten = isEaten;
}

int Pellet::GetTypePoints(const Type& t)
{
    using enum Type;
    switch (t)
    {
    case NORMAL:
        return s_typePoints.normal;
    case SUPER:
        return s_typePoints.super;
    case NONE:
        return s_typePoints.none;
    }

    throw std::invalid_argument("Invalid pellet type for getting type points.");
}

void Pellet::SetTypePoints(const TypePointValues& newValues)
{
    s_typePoints = newValues;
}

Vector2Ex<float> Pellet::GetDimensions() const
{
    return {radius, radius};
}

void Pellet::Render(Vector2Ex<float> offset) const
{
    if (m_isEaten)
        return;

    switch (m_type)
    {
    case Type::NORMAL:
        DrawCircleV(GetWorldOrigin(), radius, WHITE);
        break;
    case Type::SUPER:
        DrawCircleV(GetWorldOrigin(), radius + 2, GOLD);
        break;
    case Type::NONE:
        break;
    }
}
