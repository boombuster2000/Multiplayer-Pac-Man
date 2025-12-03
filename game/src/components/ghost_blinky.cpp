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

    if (!startNode || !endNode || startNode == endNode)
    {
        return; // No path to calculate or already there
    }

    const auto& nodes = board.GetNodes();
    std::unordered_map<Node*, float> distances;
    std::unordered_map<Node*, Node*> previous_nodes;
    for (Node* node : nodes)
    {
        distances[node] = std::numeric_limits<float>::infinity();
        previous_nodes[node] = nullptr;
    }

    distances[startNode] = 0.0f;

    auto compare = [](const std::pair<float, Node*>& a, const std::pair<float, Node*>& b) { return a.first > b.first; };

    std::priority_queue<std::pair<float, Node*>, std::vector<std::pair<float, Node*>>, decltype(compare)> pq(compare);

    pq.push({0.0f, startNode});

    while (!pq.empty())
    {
        Node* currentNode = pq.top().second;
        pq.pop();

        if (currentNode == endNode)
        {
            break; // Found the shortest path to the target
        }

        const std::array<const Arc*, 4> arcs = {&currentNode->GetUpArc(),
                                                &currentNode->GetDownArc(),
                                                &currentNode->GetLeftArc(),
                                                &currentNode->GetRightArc()};
        for (const Arc* arc : arcs)
        {
            Node* endNode = arc->GetEndNode();
            if (!endNode)
                continue;

            float weight = arc->GetLength();
            if (distances.count(currentNode) && distances[currentNode] != std::numeric_limits<float>::infinity() &&
                distances[currentNode] + weight < distances[endNode])
            {
                distances[endNode] = distances[currentNode] + weight;
                previous_nodes[endNode] = currentNode;
                pq.push({distances[endNode], endNode});
            }
        }
    }

    // Reconstruct path to find the next move
    Node* crawl = endNode;
    if (previous_nodes.find(crawl) == previous_nodes.end() && crawl != startNode)
    {
        return; // No path found
    }

    while (previous_nodes[crawl] != nullptr && previous_nodes[crawl] != startNode)
    {
        crawl = previous_nodes[crawl];
    }

    if (crawl && previous_nodes[crawl] == startNode)
    {
        // Determine direction from startNode to nextNode
        if (crawl == startNode->GetUpArc().GetEndNode())
        {
            SetQueuedDirection(UP);
        }
        else if (crawl == startNode->GetDownArc().GetEndNode())
        {
            SetQueuedDirection(DOWN);
        }
        else if (crawl == startNode->GetLeftArc().GetEndNode())
        {
            SetQueuedDirection(LEFT);
        }
        else if (crawl == startNode->GetRightArc().GetEndNode())
        {
            SetQueuedDirection(RIGHT);
        }
    }
}