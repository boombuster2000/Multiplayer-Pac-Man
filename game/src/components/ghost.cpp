#include "game/components/ghost.h"

Ghost::Ghost(const Vector2Ex<float>& spawnPosition,
             const Vector2Ex<float>& speed,
             const Vector2Ex<float>& dimensions,
             const ui::Direction& direction,
             const std::shared_ptr<Texture2D>& texture,
             const bool isReleased) :
    Entity(EntityType::GHOST, spawnPosition, speed, dimensions, direction, texture),
    m_isReleased(isReleased)
{
}

void Ghost::SetReleased(const bool isReleased)
{
    m_isReleased = isReleased;
}

bool Ghost::IsReleased() const
{
    return m_isReleased;
}

void Ghost::UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition)
{
    using enum ui::Direction;

    Node* startNode = board.GetClosestNode(GetPositionAtAnchor());
    Node* endNode = board.GetClosestNode(targetPosition);
    const NodeRouteTable& routeTable = board.GetRouteTable();

    // 1. Handle null nodes: If start or end node is invalid, cannot calculate path.
    if (!startNode || !endNode)
    {
        return;
    }

    Node* nextNode = nullptr;

    // 2. Handle case where ghost and Pacman are closest to the same node.
    // In this scenario, the ghost should try to follow Pacman along an arc.
    if (startNode == endNode)
    {
        // When ghost and Pac-Man are closest to the same node, use a simple "greedy"
        // strategy: move in the direction that closes the largest distance. This is
        // more robust than checking arcs and prevents the ghost from getting stuck.
        const Vector2Ex<float> moveVector = targetPosition - GetPositionAtAnchor();

        if (std::abs(moveVector.x) > std::abs(moveVector.y))
        {
            // Prioritize horizontal movement
            if (moveVector.x > 0)
                SetQueuedDirection(RIGHT);
            else
                SetQueuedDirection(LEFT);
        }
        else
        {
            // Prioritize vertical movement
            if (moveVector.y > 0)
            {
                SetQueuedDirection(DOWN); // Y is +ve downwards in screen coordinates
            }
            else
            {
                SetQueuedDirection(UP);
            }
        }
        return;
    }

    // 3. Standard pathfinding: startNode and endNode are different.
    Node* hop = routeTable.at(startNode).at(endNode);

    // The route table gives an intermediate node 'hop' on the path from start to end.
    // To find the actual 'nextNode' (the first step from 'startNode'), we must
    // trace the path back.
    if (hop == endNode)
    {
        // The algorithm indicates a direct path, so 'endNode' should be a neighbor.
        nextNode = endNode;
    }
    else
    {
        // The path is multi-step. We recursively look up the intermediate node
        // until we find the one that is on the direct path from startNode.
        // That node is our first hop.
        while (routeTable.at(startNode).at(hop) != hop)
        {
            hop = routeTable.at(startNode).at(hop);
        }

        nextNode = hop;
    }

    // 4. Determine direction based on nextNode.
    if (nextNode == startNode->GetUpArc().GetEndNode())
    {
        SetQueuedDirection(UP);
    }
    else if (nextNode == startNode->GetDownArc().GetEndNode())
    {
        SetQueuedDirection(DOWN);
    }
    else if (nextNode == startNode->GetLeftArc().GetEndNode())
    {
        SetQueuedDirection(LEFT);
    }
    else if (nextNode == startNode->GetRightArc().GetEndNode())
    {
        SetQueuedDirection(RIGHT);
    }
}