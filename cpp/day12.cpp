#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <array>
#include <cctype>
#include <algorithm>


struct Node
{
    static constexpr int32_t MAX_CONNECTIONS = 16;
    enum NODE_TYPE : int { START=0, END, SMALL, LARGE, INVALID };

    std::array<int32_t, MAX_CONNECTIONS> connections;
    NODE_TYPE type;

    Node() { type = INVALID; connections.fill(-1); }

    void AddConnection(int32_t other)
    {
        int32_t i = 0;
        while (connections[i] >= 0) ++i;

        connections[i] = other;
    }

    std::string name;
};

struct Graph
{
    std::unordered_map<std::string, int32_t> name2node;
    std::vector<Node> nodes;

    int32_t GetOrCreateNode(const std::string& name)
    {
        auto it = name2node.find(name);
        if (it != name2node.end())
            return it->second;

        Node new_node;
        new_node.name = name;
        if (name.compare("start") == 0) new_node.type = Node::NODE_TYPE::START;
        else if (name.compare("end") == 0) new_node.type = Node::NODE_TYPE::END;
        else if (std::islower(name[0])) new_node.type = Node::NODE_TYPE::SMALL;
        else new_node.type = Node::NODE_TYPE::LARGE;

        nodes.emplace_back(new_node);
        int32_t index = static_cast<int32_t>(nodes.size()) - 1;
        name2node.emplace(name, index);

        return index;
    }

    void AddConnection(const std::string& name0, const std::string& name1)
    {
        int32_t idx0 = GetOrCreateNode(name0);
        int32_t idx1 = GetOrCreateNode(name1);

        nodes[idx0].AddConnection(idx1);
        nodes[idx1].AddConnection(idx0);
    }
};

struct Path
{
    Path() {}
    Path(int32_t start)
    {
        steps.push_back(start);
        visited_nodes.insert(start);
    }
    Path(const Path& path, int32_t next_step) :
        steps(path.steps), visited_nodes(path.visited_nodes)
    {
        steps.push_back(next_step);
        visited_nodes.insert(next_step);
    }

    std::vector<int32_t> steps;
    std::unordered_set<int32_t> visited_nodes;
};

void FindAllPaths(const Graph& graph, std::vector<Path>& paths)
{
    std::vector<Path> paths_to_process;
    
    // start node path
    paths_to_process.emplace_back(graph.name2node.at("start"));

    while (!paths_to_process.empty())
    {
        Path path = paths_to_process.back();
        paths_to_process.pop_back();

        // take the last step and continue
        const Node& node = graph.nodes[path.steps.back()];
        int32_t connection_index = 0;
        while (node.connections[connection_index] >= 0)
        {
            const int32_t connected_node_index = node.connections[connection_index];
            const Node& connected_node = graph.nodes[connected_node_index];
            switch (connected_node.type)
            {
            case Node::END:
            {
                // finish a full path
                paths.emplace_back(path, connected_node_index);
                break;
            }
            case Node::SMALL:
            {
                // can visit small node only if it has not been visited before
                if (path.visited_nodes.count(connected_node_index) == 0)
                    paths_to_process.emplace_back(path, connected_node_index);
                break;
            }
            case Node::LARGE:
            {
                paths_to_process.emplace_back(path, connected_node_index);
                break;
            }

            case Node::START:
            case Node::INVALID:
            default:
                break;
            }

            ++connection_index;
        }
    }
}

struct Path2
{
    Path2() {}
    Path2(int32_t start)
    {
        steps.push_back(start);
        visited_nodes.insert(start);
        have_visited_special_twice = false;
    }
    Path2(const Path2& path, int32_t next_step) :
        steps(path.steps), visited_nodes(path.visited_nodes), 
        have_visited_special_twice(path.have_visited_special_twice)
    {
        steps.push_back(next_step);
        visited_nodes.insert(next_step);
    }

    Path2(const Path2& path, int32_t next_step, bool _have_visited_special_twice) :
        steps(path.steps), visited_nodes(path.visited_nodes),
        have_visited_special_twice(_have_visited_special_twice)
    {
        steps.push_back(next_step);
        visited_nodes.insert(next_step);
    }

    // keep track of one special small cave
    bool have_visited_special_twice;
    std::vector<int32_t> steps;
    std::unordered_set<int32_t> visited_nodes;

    static bool Less(const Path2& p0, const Path2& p1)
    {
        if (p0.steps.size() < p1.steps.size())
            return true;
        else if (p1.steps.size() < p0.steps.size())
            return false;

        for (size_t i = 0; i < p0.steps.size(); ++i)
        {
            if (p0.steps[i] < p1.steps[i])
                return true;
            else if (p0.steps[i] > p1.steps[i])
                return false;
        }

        return false;
    }

    static bool Equal(const Path2& p0, const Path2& p1)
    {
        if (p0.steps.size() != p1.steps.size())
            return false;

        for (size_t i = 0; i < p0.steps.size(); ++i)
            if (p0.steps[i] != p1.steps[i])
                return false;

        return true;
    }
};

void FindAllPaths(const Graph& graph, std::vector<Path2>& paths, int32_t special_small)
{
    std::vector<Path2> paths_to_process;

    // start node path
    paths_to_process.emplace_back(graph.name2node.at("start"));

    while (!paths_to_process.empty())
    {
        Path2 path = paths_to_process.back();
        paths_to_process.pop_back();

        // take the last step and continue
        const Node& node = graph.nodes[path.steps.back()];
        int32_t connection_index = 0;
        while (node.connections[connection_index] >= 0)
        {
            const int32_t connected_node_index = node.connections[connection_index];
            const Node& connected_node = graph.nodes[connected_node_index];
            switch (connected_node.type)
            {
            case Node::END:
            {
                // finish a full path
                paths.emplace_back(path, connected_node_index);
                break;
            }
            case Node::SMALL:
            {
                if (connected_node_index == special_small)
                {
                    if (path.visited_nodes.count(connected_node_index) == 0)
                        // first visit
                        paths_to_process.emplace_back(path, connected_node_index);
                    else if (!path.have_visited_special_twice)
                        // second visit
                        paths_to_process.emplace_back(path, connected_node_index, true);
                }
                else
                {
                    // can visit small node only if it has not been visited before
                    if (path.visited_nodes.count(connected_node_index) == 0)
                        paths_to_process.emplace_back(path, connected_node_index);
                }
                break;
            }
            case Node::LARGE:
            {
                paths_to_process.emplace_back(path, connected_node_index);
                break;
            }

            case Node::START:
            case Node::INVALID:
            default:
                break;
            }

            ++connection_index;
        }
    }
}

int32_t day12_part1(const char* input_path)
{
    Graph graph;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            size_t split = line.find('-');
            std::string name0 = line.substr(0, split);
            std::string name1 = line.substr(split + 1);

            graph.AddConnection(name0, name1);
        }
        input_file.close();
    }

    std::vector<Path> paths;
    FindAllPaths(graph, paths);

//     for (const Path& p : paths)
//     {
//         for (int32_t ni : p.steps)
//             std::cout << graph.nodes[ni].name << ", ";
//         std::cout << "\n";
//     }

    return static_cast<int32_t>(paths.size());
}

int32_t day12_part2(const char* input_path)
{
    Graph graph;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            size_t split = line.find('-');
            std::string name0 = line.substr(0, split);
            std::string name1 = line.substr(split + 1);

            graph.AddConnection(name0, name1);
        }
        input_file.close();
    }

    std::vector<Path2> paths;
    for (int32_t i = 0; i < graph.nodes.size(); ++i)
    {
        if (graph.nodes[i].type == Node::SMALL)
            FindAllPaths(graph, paths, i);
    }

    std::sort(paths.begin(), paths.end(), Path2::Less);
    auto last = std::unique(paths.begin(), paths.end(), Path2::Equal);
    paths.erase(last, paths.end());

//     for (const Path2& p : paths)
//     {
//         for (int32_t ni : p.steps)
//             std::cout << graph.nodes[ni].name << ", ";
//         std::cout << "\n";
//     }

    return static_cast<int32_t>(paths.size());
}
