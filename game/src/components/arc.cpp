#include "game/components/node_system.h"

Arc::Arc(Node* startNode, Node* endNode) :
    m_startNode(startNode),
    m_endNode(endNode)
{
}

Node* Arc::GetStartNode() const
{
    return m_startNode;
}

Node* Arc::GetEndNode() const
{
    return m_endNode;
}

float Arc::GetLength() const
{
    return (m_startNode->GetPosition() - m_endNode->GetPosition()).GetLength();
}

const std::vector<Entity*>& Arc::GetEntities() const
{
    return m_entities;
}

void Arc::AddEntity(Entity* entity)
{
    m_entities.push_back(entity);
}