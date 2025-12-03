#include "game/components/board.h"
#include "engine/serialization/json_converters.hpp"
#include "game/components/pellet.h"
#include "game/file_paths.h"
#include "game/serialization/json_converters.hpp"
#include <filesystem>
#include <fstream>
#include <string_view>

Board::Board() :
    Grid(Vector2Ex<size_t>(14, 14),
         Vector2Ex<float>(50, 50),
         Vector2Ex<float>(static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) / 2),
         ui::AnchorPoint::MIDDLE,
         Vector2Ex<float>(0, 0),
         Tile::Type::PATH,
         Pellet::Type::NORMAL,
         Vector2Ex<float>(0, 0),
         Vector2Ex<float>(50, 50)),
    m_name("test-file")
{
    using enum Tile::Type;
    AddBoundaries();

    // Row 2 - top horizontal walls
    for (size_t x : {2, 4, 5, 6, 7, 8, 9, 10, 11})
        SetTileType({x, 2}, WALL);

    // Row 3 - vertical pillars
    for (size_t x : {2, 11})
        SetTileType({x, 3}, WALL);

    // Row 4 - inner walls with gaps
    for (size_t x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 4}, WALL);

    // Row 5 - ghost house top
    for (size_t x : {0, 2, 4, 11})
        SetTileType({x, 5}, WALL);

    // Row 6 - ghost house sides
    for (size_t x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 6}, WALL);

    // Row 7 - ghost house sides
    for (size_t x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 7}, WALL);

    // Row 8 - ghost house bottom
    for (size_t x : {0, 2, 4, 9, 11})
        SetTileType({x, 8}, WALL);

    // Row 9 - inner walls with gaps
    for (size_t x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 9}, WALL);

    // Row 10 - vertical pillars
    for (size_t x : {2, 11})
        SetTileType({x, 10}, WALL);

    // Row 11 - bottom horizontal walls
    for (size_t x : {2, 3, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 11}, WALL);

    CreateNodesAndArcs();
}

Board::Board(std::string_view filename)
{
    *this = Board::LoadFromFile(filename);
}

const std::string& Board::GetName() const
{
    return m_name;
}

const std::unordered_map<Vector2Ex<size_t>, Node*>& Board::GetNodeMap() const
{
    return m_nodeMap;
}

std::vector<Node*> Board::GetNodes() const
{
    return m_nodes;
}

bool Board::HasLineOfSight(const Vector2Ex<float>& pos1, const Vector2Ex<float>& pos2) const
{
    Vector2Ex<float> line = pos2 - pos1;
    float length = line.GetLength();

    if (length == 0.0f)
        return true;

    Vector2Ex<float> direction = line / length;

    // Walk along the line in small steps (e.g., 1 pixel at a time)
    for (float i = 0.0f; i < length; i += 1.0f)
    {
        Vector2Ex<float> currentPoint = pos1 + direction * i;
        const Tile& tile = GetTileFromPosition(currentPoint);
        if (tile.GetType() == Tile::Type::WALL)
            return false;
    }

    // Check the final point as well to be safe
    const Tile& endTile = GetTileFromPosition(pos2);
    if (endTile.GetType() == Tile::Type::WALL)
        return false;

    return true;
}

Node* Board::GetClosestNode(const Vector2Ex<float> position) const
{
    if (m_nodes.empty())
    {
        return nullptr;
    }

    Node* closestNode = nullptr;
    float smallestDistanceSq = std::numeric_limits<float>::max();

    for (const auto& node : m_nodes)
    {
        // Only consider nodes with a clear line of sight
        if (!HasLineOfSight(position, node->GetPosition()))
        {
            continue;
        }

        float distanceSq = (position - node->GetPosition()).GetLengthSqr();
        if (distanceSq < smallestDistanceSq)
        {
            smallestDistanceSq = distanceSq;
            closestNode = node;
        }
    }

    // Fallback: if no node has LOS, find the closest one regardless of walls.
    // This can prevent entities from getting stuck if they are in a bad spot.
    if (closestNode == nullptr)
    {
        for (const auto& node : m_nodes)
        {
            float distanceSq = (position - node->GetPosition()).GetLengthSqr();
            if (distanceSq < smallestDistanceSq)
            {
                smallestDistanceSq = distanceSq;
                closestNode = node;
            }
        }
    }

    return closestNode;
}

const NodeRouteTable& Board::GetRouteTable() const
{
    return m_routeTable;
}

void Board::SetTileType(const Vector2Ex<size_t>& index, const Tile::Type& type)
{
    Grid::GetTile(index).SetType(type);
}

void Board::SaveToFile() const
{
    json j = *this;

    const std::filesystem::path& boardFolder = FilePaths::s_boardsDirectory;
    const std::string filename = m_name + std::string(".json");

    std::ofstream file(boardFolder / filename);
    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
}

void Board::SaveHighscoresToFile() const
{
    const std::filesystem::path& boardFolder = FilePaths::s_boardsDirectory;
    const std::filesystem::path filename = m_name + std::string(".json");

    Board originalBoard = Board::LoadFromFile(boardFolder / filename);

    for (const auto& [profileName, score] : m_highScores)
    {
        originalBoard.SetHighscore(profileName, score);
    }

    json j = originalBoard;

    std::ofstream file(boardFolder / filename);

    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
}

HighscoreMap Board::GetHighscores() const
{
    return m_highScores;
}

void Board::SetHighscore(std::string_view profileName, int score)
{
    if (auto it = m_highScores.find(profileName); it != m_highScores.end())
        // Profile exists, update if score is higher
        if (score > it->second)
            it->second = score;

        else
            // Profile doesn't exist, insert new score
            m_highScores.emplace(profileName, score);
}

Board Board::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::filesystem::filesystem_error("Failed to open file",
                                                std::filesystem::path(filename),
                                                std::error_code{});

    json j;
    file >> j;
    file.close();
    Board board = j.get<Board>();
    return board;
}

Board Board::LoadFromFile(const std::filesystem::path& filepath)
{
    return Board::LoadFromFile(filepath.string());
}

void Board::AddBoundaries()
{
    using enum Tile::Type;
    const Vector2Ex<size_t>& boardSize = GetGridSize();

    for (int x = 0; x < boardSize.x; x++)
    {
        GetTile(0, x).SetType(WALL);
        GetTile(boardSize.y - 1, x).SetType(WALL);
    }

    for (int y = 0; y < boardSize.y; y++)
    {
        GetTile(y, 0).SetType(WALL);
        GetTile(y, boardSize.x - 1).SetType(WALL);
    }
}

void Board::AddArcsToNode(Node* node, const Vector2Ex<size_t>& index)
{
    using enum ui::Direction;

    // Connect to node on the RIGHT
    Vector2Ex<size_t> rightIndex = GetIndexOfNextJunction(index, RIGHT);
    if (rightIndex != index)
    {
        Node* rightNode = m_nodeMap.at(rightIndex);
        node->SetRightArc({node, rightNode});
    }

    // Connect to node on the LEFT
    Vector2Ex<size_t> leftIndex = GetIndexOfNextJunction(index, LEFT);
    if (leftIndex != index)
    {
        Node* leftNode = m_nodeMap.at(leftIndex);
        node->SetLeftArc({node, leftNode});
    }

    // Connect to node BELOW
    Vector2Ex<size_t> downIndex = GetIndexOfNextJunction(index, DOWN);
    if (downIndex != index)
    {
        Node* downNode = m_nodeMap.at(downIndex);
        node->SetDownArc({node, downNode});
    }

    // Connect to node ABOVE
    Vector2Ex<size_t> upIndex = GetIndexOfNextJunction(index, UP);
    if (upIndex != index)
    {
        Node* upNode = m_nodeMap.at(upIndex);
        node->SetUpArc({node, upNode});
    }
}

void Board::CreateNodes()
{
    for (size_t y = 0; y < GetGridSize().y; y++)
    {
        for (size_t x = 0; x < GetGridSize().x; x++)
        {
            const Vector2Ex<size_t> index(x, y);
            if (IsTileJunction(index))
            {
                m_nodeMap[index] = new Node(GetPositionFromIndex(index), index);
                m_nodes.push_back(m_nodeMap[index]);
            }
        }
    }
}

void Board::CreateDistanceTable()
{
    auto fillCell = [](std::unordered_map<Node*, float>& cell, const Arc& arc) {
        if (arc.GetEndNode() != nullptr)
            cell[arc.GetEndNode()] = arc.GetLength();
    };

    for (Node* currentNode : m_nodes)
    {
        std::unordered_map<Node*, float>& row = m_distanceTable[currentNode];

        // Fill arc neighbors
        fillCell(row, currentNode->GetUpArc());
        fillCell(row, currentNode->GetDownArc());
        fillCell(row, currentNode->GetLeftArc());
        fillCell(row, currentNode->GetRightArc());

        // Distance to itself
        row[currentNode] = 0.f;

        // All other nodes = ∞ unless filled
        for (Node* node : m_nodes)
        {
            if (!row.count(node))
            {
                row[node] = std::numeric_limits<float>::infinity();
            }
        }
    }
}

void Board::CreateRouteTable()
{
    for (const auto& currentNode : m_nodes)
    {
        for (const auto& node : m_nodes)
        {
            m_routeTable[currentNode][node] = node;
        }
    }
}

void Board::Floyds()
{
    // row and col are the same

    for (const auto& [node, _] : m_distanceTable)
    {
        for (const auto& [yNode, row] : m_distanceTable)
        {
            for (const auto& [xNode, distance] : row)
            {
                float x = m_distanceTable[node][xNode];
                float y = m_distanceTable[yNode][node];
                float z = m_distanceTable[yNode][xNode];
                if (x + y < z)
                {
                    m_distanceTable[yNode][xNode] = x + y;
                    m_routeTable[yNode][xNode] = node;
                }
            }
        }
    }
}

bool Board::IsTileJunction(const Vector2Ex<size_t>& index) const
{
    if (GetTile(index).GetType() != Tile::Type::PATH)
    {
        return false;
    }

    const auto isPath = [&](size_t y, size_t x) {
        return IsValidIndex({x, y}) && GetTile({x, y}).GetType() == Tile::Type::PATH;
    };

    const bool up = isPath(index.y - 1, index.x);
    const bool down = isPath(index.y + 1, index.x);
    const bool left = isPath(index.y, index.x - 1);
    const bool right = isPath(index.y, index.x + 1);

    // It's a straight corridor if it has exactly two exits opposite each other.
    const bool isHorizontalCorridor = left && right && !up && !down;
    const bool isVerticalCorridor = up && down && !left && !right;

    // A junction is any path tile that is NOT part of a straight corridor.
    return !(isHorizontalCorridor || isVerticalCorridor) || (isHorizontalCorridor && isVerticalCorridor);
}

Vector2Ex<size_t> Board::GetIndexOfNextJunction(const Vector2Ex<size_t>& startIndex,
                                                const ui::Direction& direction) const
{
    const Vector2Ex<int> step = Vector2Ex<int>::GetDirectionVector(direction);
    Vector2Ex<size_t> steppedIndex = startIndex;

    while (true)
    {
        steppedIndex += step;

        const Tile& tile = GetTile(steppedIndex);

        if (tile.GetType() == Tile::Type::WALL)
            return steppedIndex - step; // hit a wall, return last valid

        if (IsTileJunction(steppedIndex))
            return steppedIndex; // found junction
    }
}

void Board::CreateNodesAndArcs()
{
    CreateNodes();

    for (auto const& [index, node] : m_nodeMap)
        AddArcsToNode(node, index);

    CreateDistanceTable();
    CreateRouteTable();
    Floyds();
}

Vector2Ex<float> Board::GetPlayerSpawnPoint() const
{
    return GetPositionFromIndex({1, 1});
}

Vector2Ex<float> Board::GetSpeedyGhostSpawnPoint() const
{
    return GetPositionFromIndex({5, 8});
}
