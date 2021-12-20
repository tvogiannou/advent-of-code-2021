#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>


static int32_t HashPair(char cL, char cR)
{ 
    return ((int32_t)cL << 16) | (int32_t)cR; 
}

static void ExtractPair(int32_t key, char& cL, char& cR)
{
    cR = key & 0xFF;
    cL = (key >> 16) & 0xFF;
}

void CountCharInsertions(
    const int32_t num_steps, 
    const std::string& polymer_start, 
    const std::unordered_map<int32_t, char> &pair_rules,
    const std::unordered_map<int32_t, uint64_t> pair_counts, 
    std::unordered_map<char, uint64_t>& char_counts)
{
    // init char counts
    for (char c : polymer_start)
        char_counts.at(c) += 1;

    // init pair counts
    auto cur_pair_counts = pair_counts;
    for (size_t i = 0; i < polymer_start.size() - 1; ++i)
    {
        char cL = polymer_start[i];
        char cR = polymer_start[i + 1];

        const int32_t key = HashPair(cL, cR);
        cur_pair_counts.at(key) += 1;
    }

    // advance steps
    for (int32_t i = 0; i < num_steps; ++i)
    {
        auto new_pair_counts = pair_counts;
        for (auto iter = cur_pair_counts.begin(); iter != cur_pair_counts.end(); ++iter)
        {
            const uint64_t pair_count = iter->second;
            const int32_t key = iter->first;
            char cL, cR;
            ExtractPair(key, cL, cR);

            // count newly inserted chars
            char cNew = pair_rules.at(key);
            char_counts.at(cNew) += pair_count;

            // keep track of new pairs
            const int32_t keyL = HashPair(cL, cNew);
            const int32_t keyR = HashPair(cNew, cR);
            new_pair_counts.at(keyL) += pair_count;
            new_pair_counts.at(keyR) += pair_count;
        }

        cur_pair_counts.swap(new_pair_counts);
    }
}

uint64_t day14_part1(const char* input_path)
{
    uint64_t count = 0;
    std::string polymer_start;
    std::unordered_map<int32_t, uint64_t> pair_counts;
    std::unordered_map<char, uint64_t> char_counts;
    std::unordered_map<int32_t, char> pair_rules;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        // read polymer template starting point
        std::getline(input_file, polymer_start);

        std::string line;
        std::getline(input_file, line); // empty line
        
        // read rules
        while(std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char cL, cR, cNew, delim;
            iss >> cL >> cR >> delim >> delim >> cNew;

            const int32_t key = HashPair(cL, cR);
            pair_counts.emplace(key, 0u);
            pair_rules.emplace(key, cNew);

            char_counts[cL] = 0;
            char_counts[cR] = 0;
        }
        input_file.close();
    }

    CountCharInsertions(10, polymer_start, pair_rules, pair_counts, char_counts);

//     for (auto iter = char_counts.begin(); iter != char_counts.end(); ++iter)
//         std::cout << iter->first << ": " << iter->second << "\n";

    uint64_t least_common = char_counts.begin()->second;
    uint64_t most_common = least_common;
    for (auto iter = char_counts.begin(); iter != char_counts.end(); ++iter)
    {
        least_common = std::min(least_common, iter->second);
        most_common = std::max(most_common, iter->second);
    }

    return most_common - least_common;
}


uint64_t day14_part2(const char* input_path)
{
    const int32_t num_steps = 40;

    uint64_t count = 0;
    std::string polymer_start;
    std::unordered_map<int32_t, uint64_t> pair_counts;
    std::unordered_map<char, uint64_t> char_counts;
    std::unordered_map<int32_t, char> pair_rules;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        // read polymer template starting point
        std::getline(input_file, polymer_start);

        std::string line;
        std::getline(input_file, line); // empty line

        // read rules
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char cL, cR, cNew, delim;
            iss >> cL >> cR >> delim >> delim >> cNew;

            const int32_t key = HashPair(cL, cR);
            pair_counts.emplace(key, 0u);
            pair_rules.emplace(key, cNew);

            char_counts[cL] = 0;
            char_counts[cR] = 0;
        }
        input_file.close();
    }

    CountCharInsertions(40, polymer_start, pair_rules, pair_counts, char_counts);

//    for (auto iter = char_counts.begin(); iter != char_counts.end(); ++iter)
//        std::cout << iter->first << ": " << iter->second << "\n";

    uint64_t least_common = char_counts.begin()->second;
    uint64_t most_common = least_common;
    for (auto iter = char_counts.begin(); iter != char_counts.end(); ++iter)
    {
        least_common = std::min(least_common, iter->second);
        most_common = std::max(most_common, iter->second);
    }

    return most_common - least_common;

    return 0;
}
