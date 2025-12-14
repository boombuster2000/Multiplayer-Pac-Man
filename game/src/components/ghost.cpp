#include "game/components/ghost.h"
#include "game/components/board.h"
#include "game/components/node_system.h"
#include <limits>

Ghost::Ghost(const Vector2Ex<float>& spawnPosition,
             const Vector2Ex<float>& speed,
             const Vector2Ex<float>& dimensions,
             const ui::Direction& direction,
             const Type ghostType,
             const Vector2Ex<float>& guardPosition,
             const float releaseTime,
             const std::shared_ptr<Texture2D>& texture,
             const State state) :
    Entity(EntityType::GHOST, spawnPosition, speed, dimensions, direction, texture),
    m_defaultTexture(texture),
    m_state(state),
    m_ghostType(ghostType),
    m_guardPosition(guardPosition),
    m_releaseTime(releaseTime)
{
}

Ghost::State Ghost::GetState() const
{
    return m_state;
}

void Ghost::SetState(const State state)
{
    m_state = state;
}

Ghost::Type Ghost::GetType() const
{
    return m_ghostType;
}

float Ghost::GetReleaseTime() const
{
    return m_releaseTime;
}

void Ghost::SetReleaseTime(const float releaseTime)
{
    m_releaseTime = releaseTime;
}
void Ghost::DecreaseReleaseTime(const float reduction)
{
    m_releaseTime -= reduction;
}

Vector2Ex<float> Ghost::GetGuardPosition() const
{
    return m_guardPosition;
}

void Ghost::SetGuardPosition(const Vector2Ex<float>& guardPosition)
{
    m_guardPosition = guardPosition;
}

bool Ghost::WasFrightened() const
{
    return m_wasFrightened;
}

void Ghost::SetWasFrightened(const bool wasFrightened)
{
    m_wasFrightened = wasFrightened;
}
void Ghost::ResetTexture()
{
    SetTexture(m_defaultTexture);
}

void Ghost::UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition)
{
    using enum ui::Direction;

    // Direction update handled by game when ghosts frightened
    if (m_state == State::FRIGHTENED)
        return;

    Node* startNode = board.GetClosestNode(GetPositionAtAnchor());
    Node* endNode = board.GetClosestNode(targetPosition);
    const NodeRouteTable& routeTable = board.GetRouteTable();

    if (!startNode || !endNode)
        return;

    if (m_state == State::SCATTER && !Board::IsAtNode(GetPositionAtAnchor(), startNode->GetPosition()))
        return;

    constexpr std::array<ui::Direction, 4> priority = {UP, LEFT, DOWN, RIGHT};

    auto NeighborForDir = [&](const ui::Direction d) -> Node* {
        switch (d)
        {
        case UP:
            return startNode->GetUpArc().GetEndNode();
        case DOWN:
            return startNode->GetDownArc().GetEndNode();
        case LEFT:
            return startNode->GetLeftArc().GetEndNode();
        case RIGHT:
            return startNode->GetRightArc().GetEndNode();
        }
        return nullptr;
    };

    // ----------------------------------------------
    // Case 1: startNode == endNode (same tile)
    // ----------------------------------------------
    if (startNode == endNode)
    {
        // Scatter special case: use priority only if AT TARGET AND ghost is stationary
        if (m_state == State::SCATTER && IsStationary())
        {
            for (const ui::Direction d : priority)
            {
                if (ui::IsOppositeDirection(GetDirection(), d))
                    continue;
                if (NeighborForDir(d))
                {
                    SetQueuedDirection(d);
                    return;
                }
            }
            SetQueuedDirection(ui::GetOppositeDirection(GetDirection()));
            return;
        }

        // Non-stationary scatter or non-scatter: greedy fallback
        const Vector2Ex<float> mv = targetPosition - GetPositionAtAnchor();
        const ui::Direction d = (std::abs(mv.x) > std::abs(mv.y)) ? (mv.x > 0 ? RIGHT : LEFT) : (mv.y > 0 ? DOWN : UP);

        SetQueuedDirection(d);
        return;
    }

    // ----------------------------------------------
    // Case 2: Pathfinding for start != end
    // ----------------------------------------------
    Node* hop = routeTable.at(startNode).at(endNode);
    Node* nextNode = nullptr;

    if (hop == endNode)
    {
        nextNode = endNode;
    }
    else
    {
        while (routeTable.at(startNode).at(hop) != hop)
            hop = routeTable.at(startNode).at(hop);
        nextNode = hop;
    }

    ui::Direction proposed = GetDirection();

    if (nextNode == startNode->GetUpArc().GetEndNode())
        proposed = UP;
    else if (nextNode == startNode->GetDownArc().GetEndNode())
        proposed = DOWN;
    else if (nextNode == startNode->GetLeftArc().GetEndNode())
        proposed = LEFT;
    else if (nextNode == startNode->GetRightArc().GetEndNode())
        proposed = RIGHT;

    // -------------------------------------------------------
    // Scatter: If proposed direction is reverse, find next-shortest path.
    // -------------------------------------------------------
    if (m_state == State::SCATTER && ui::IsOppositeDirection(GetDirection(), proposed))
    {
        ui::Direction bestDir = ui::GetOppositeDirection(GetDirection());
        float bestDist = std::numeric_limits<float>::infinity();

        for (const ui::Direction d : {UP, DOWN, LEFT, RIGHT})
        {
            if (ui::IsOppositeDirection(GetDirection(), d))
                continue;

            const Node* n = NeighborForDir(d);
            if (!n)
                continue;

            if (const float dist = (n->GetPosition() - endNode->GetPosition()).GetLengthSqr(); dist < bestDist)
            {
                bestDist = dist;
                bestDir = d;
            }
        }

        if (bestDir != GetOppositeDirection(GetDirection()))
        {
            SetQueuedDirection(bestDir);
            return;
        }

        SetQueuedDirection(proposed);
        return;
    }

    SetQueuedDirection(proposed);
}
