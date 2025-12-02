#include "game/components/node_system.h"

Node::Node(const Vector2Ex<float>& position, const Vector2Ex<size_t>& index) :
    m_position(position),
    m_index(index)
{
}

const Arc& Node::GetLeftArc() const
{
    return m_left;
}

void Node::SetLeftArc(const Arc& arc)
{
    m_left = arc;
}

const Arc& Node::GetRightArc() const
{
    return m_right;
}

void Node::SetRightArc(const Arc& arc)
{
    m_right = arc;
}

const Arc& Node::GetUpArc() const
{
    return m_up;
}

void Node::SetUpArc(const Arc& arc)
{
    m_up = arc;
}

const Arc& Node::GetDownArc() const
{
    return m_down;
}

void Node::SetDownArc(const Arc& arc)
{
    m_down = arc;
}

const Vector2Ex<float>& Node::GetPosition() const
{
    return m_position;
}

const Vector2Ex<size_t>& Node::GetIndex() const
{
    return m_index;
}
