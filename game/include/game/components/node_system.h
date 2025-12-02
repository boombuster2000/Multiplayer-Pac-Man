#pragma once
#include "engine/core/vector2ex.h"
#include <vector>

class Node;

class Arc
{
  private:
    Node* m_startNode = nullptr;
    Node* m_endNode = nullptr;

  public:
    Arc() = default;
    Arc(Node* startNode, Node* endNode);

    Node* GetStartNode() const;
    Node* GetEndNode() const;
};

class Node
{
  private:
    Arc m_left;
    Arc m_right;
    Arc m_up;
    Arc m_down;

    Vector2Ex<float> m_position;
    Vector2Ex<size_t> m_index;

  public:
    Node(const Vector2Ex<float>& position, const Vector2Ex<size_t>& index);
    Node() = default;

    const Arc& GetLeftArc() const;
    void SetLeftArc(const Arc& arc);

    const Arc& GetRightArc() const;
    void SetRightArc(const Arc& arc);

    const Arc& GetUpArc() const;
    void SetUpArc(const Arc& arc);

    const Arc& GetDownArc() const;
    void SetDownArc(const Arc& arc);

    const Vector2Ex<float>& GetPosition() const;

    const Vector2Ex<size_t>& GetIndex() const;
};