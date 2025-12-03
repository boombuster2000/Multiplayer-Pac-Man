#include "game/components/ghost_blinky.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/enums.h"
#include "game/components/node_system.h"
#include "game/game_application.h"
#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

Blinky::Blinky(const Vector2Ex<float>& spawnPosition,
               const Vector2Ex<float>& speed,
               const Vector2Ex<float>& dimensions,
               const ui::Direction& direction) :
    Ghost(spawnPosition,
          speed,
          dimensions,
          direction,
          game::GameApplication::Get().GetTexturesManager().GetTexture("blinky"))
{
}

void Blinky::UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition)
{
    using enum ui::Direction;

    Node* startNode = board.GetClosestNode(GetPositionAtAnchor());
    Node* endNode = board.GetClosestNode(targetPosition);
    const NodeRouteTable& routeTable = board.GetRouteTable();

    if (!startNode || !endNode || startNode == endNode)
    {
        return; // No path to calculate or already there
    }

    Node* nextNode = nullptr;
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

    // Determine direction from startNode to nextNode
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