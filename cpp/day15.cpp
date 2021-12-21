#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <assert.h>


using Index4 = std::array<int32_t, 4>;
int32_t GetNeighbours(Index4& neighbours, int32_t index, int32_t row_size, int32_t num_cells)
{
    int32_t num_neightbours = 0;

    const int32_t rem = index % row_size;
    const bool left = rem > 0;
    const bool right = rem < row_size - 1;
    const bool up = index >= row_size;
    const bool down = index < num_cells - row_size;

    if (left)
        neighbours[num_neightbours++] = index - 1;
    if (right)
        neighbours[num_neightbours++] = index + 1;
    if (up)
        neighbours[num_neightbours++] = index - row_size;
    if (down)
        neighbours[num_neightbours++] = index + row_size;

    return num_neightbours;
}

struct CellInfo
{
    int32_t came_from = -1;
    int32_t cost = -1;
};

int32_t FindBestPathCost(int32_t start, int32_t goal, int32_t row_size,
    const std::vector<uint8_t>& risk_level)
{
    std::vector<CellInfo> track;
    track.resize(risk_level.size());

    // custom priority queue where priority gets the node with the lower cost
    auto cmp = [&track](int32_t l, int32_t r) { return track[l].cost > track[r].cost; };
    std::priority_queue<int32_t, std::vector<int32_t>, decltype(cmp)> frontier(cmp);

    // start from the end and get to the start to simplify path tracking
    track[goal].cost = risk_level[goal];
    frontier.emplace(goal);

    // Dijkstra's algorithm
    while (!frontier.empty())
    {
        int32_t current = frontier.top();
        frontier.pop();

        if (current == start)
            break;

        Index4 neighbours;
        int32_t num_neighs = 
            GetNeighbours(neighbours, current, row_size, static_cast<int32_t>(risk_level.size()));
        for (int32_t i = 0; i < num_neighs; ++i)
        {
            const int32_t n = neighbours[i];
            const int32_t new_cost = track[current].cost + risk_level[n];
            
            if (track[n].cost < 0 || new_cost < track[n].cost)
            {
                track[n].cost = new_cost;
                track[n].came_from = current;
                frontier.emplace(n);
            }
        }
    }

    // the total cost is 
    return track[track[start].came_from].cost;
}

struct ExpandedMap
{
    static constexpr int32_t N = 5;

    int32_t top_left_row_size;
    int32_t top_left_col_size;
    const std::vector<uint8_t>& risk_level;

    ExpandedMap(const std::vector<uint8_t>& _risk_level) : risk_level(_risk_level) {}

    int32_t GetFullSize() const { return N * N * static_cast<int32_t>(risk_level.size()); }
    int32_t GetRowSize() const { return top_left_row_size * N; }

    uint8_t GetRisk(int32_t index) const
    {
        const int32_t row_size = GetRowSize();
        const int32_t expanded_i = index % row_size;
        const int32_t expanded_j = index / row_size;

        // compute how much to add to the value
        const int32_t rep_i = expanded_i / top_left_row_size;
        const int32_t rep_j = expanded_j / top_left_col_size;
        const int32_t added_val = rep_i + rep_j;

        // compute the position in the top left
        const int32_t top_left_i = expanded_i % top_left_row_size;
        const int32_t top_left_j = expanded_j % top_left_col_size;
        const int32_t top_left_val = risk_level[top_left_j * top_left_row_size + top_left_i];

        int32_t final_val = top_left_val + added_val;
        if (final_val > 9) final_val -= 9;

        return final_val;
    }
};

int32_t FindBestPathCost(int32_t start, int32_t goal, const ExpandedMap& expanded_map)
{
    std::unordered_map<int32_t, CellInfo> track;

    // custom priority queue where priority gets the node with the lower cost
    auto cmp = [&track](int32_t l, int32_t r) { return track[l].cost > track[r].cost; };
    std::priority_queue<int32_t, std::vector<int32_t>, decltype(cmp)> frontier(cmp);

    // start from the end and get to the start to simplify path tracking
    track[goal].cost = expanded_map.GetRisk(goal);
    frontier.emplace(goal);

    // Dijkstra's algorithm
    while (!frontier.empty())
    {
        int32_t current = frontier.top();
        frontier.pop();

        if (current == start)
            break;

        Index4 neighbours;
        int32_t num_neighs =
            GetNeighbours(neighbours, current, expanded_map.GetRowSize(), expanded_map.GetFullSize());
        for (int32_t i = 0; i < num_neighs; ++i)
        {
            const int32_t n = neighbours[i];
            const int32_t new_cost = track[current].cost + expanded_map.GetRisk(n);

            if (track[n].cost < 0 || new_cost < track[n].cost)
            {
                track[n].cost = new_cost;
                track[n].came_from = current;
                frontier.emplace(n);
            }
        }
    }

    // the total cost is 
    return track[track[start].came_from].cost;
}

int32_t day15_part1(const char* input_path)
{
    std::vector<uint8_t> risk_level;
    risk_level.reserve(1024);
    int32_t row_length = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;

        // use first line to compute row length
        {
            std::getline(input_file, line);
            std::istringstream iss(line);
            char val;
            while (iss >> val)
            {
                // convert ASCII code for 0-9 to number
                risk_level.emplace_back((uint8_t)val - 48);
                ++row_length;
            }
        }

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char val;
            while (iss >> val)
                // convert ASCII code for 0-9 to number
                risk_level.emplace_back((uint8_t)val - 48);
        }
        input_file.close();
    }

    int32_t cost = FindBestPathCost(
        0, static_cast<int32_t>(risk_level.size()) - 1, row_length, risk_level);

    return cost;
}


int32_t day15_part2(const char* input_path)
{
    std::vector<uint8_t> risk_level;
    risk_level.reserve(1024);
    int32_t row_length = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;

        // use first line to compute row length
        {
            std::getline(input_file, line);
            std::istringstream iss(line);
            char val;
            while (iss >> val)
            {
                // convert ASCII code for 0-9 to number
                risk_level.emplace_back((uint8_t)val - 48);
                ++row_length;
            }
        }

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char val;
            while (iss >> val)
                // convert ASCII code for 0-9 to number
                risk_level.emplace_back((uint8_t)val - 48);
        }
        input_file.close();
    }

    ExpandedMap expandedMap(risk_level);
    expandedMap.top_left_row_size = row_length;
    expandedMap.top_left_col_size = static_cast<int32_t>(risk_level.size()) / row_length;

//     for (int32_t i = 0; i < expandedMap.GetFullSize() / expandedMap.GetRowSize(); ++i)
//     {
//         for (int32_t j = 0; j < expandedMap.GetRowSize(); j++)
//             std::cout << (int32_t)expandedMap.GetRisk(i * expandedMap.GetRowSize() + j);
//         std::cout << "\n";
//     }

    int32_t cost = FindBestPathCost(0, expandedMap.GetFullSize() - 1, expandedMap);

    return cost;
}
