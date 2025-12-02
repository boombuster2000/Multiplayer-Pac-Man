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