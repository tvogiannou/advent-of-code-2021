#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>


constexpr int32_t N = 5;
using TableType = std::array<int32_t, N*N>;


int32_t FindIndex(const std::vector<int32_t>& v, int32_t x)
{
    for (int32_t i = 0; i < static_cast<int32_t>(v.size()); ++i)
        if (v[i] == x)
            return i;

    return -1;
}

int32_t FindIndex(const TableType& v, int32_t x)
{
    for (int32_t i = 0; i < static_cast<int32_t>(v.size()); ++i)
        if (v[i] == x)
            return i;

    return -1;
}

bool ReadTable(std::ifstream& input_file, TableType& table)
{
    std::string line;
    int32_t table_index = 0;
    for (int32_t i = 0; i < N; ++i)
    {
        std::getline(input_file, line);
        std::istringstream iss(line);
        uint32_t res;
        while (iss >> res)
            table[table_index++] = res;
    }

    return table_index == table.size();
}

int32_t GetEarliestBingo(const TableType& table, const std::vector<int32_t>& numbers)
{
    int32_t col_scores[N] = {}; // column scores as being read
    int32_t bingo_score = static_cast<int32_t>(numbers.size());

    // read per row
    for (int32_t i = 0; i < N; ++i)
    {
        int32_t row_score = 0;
        int32_t col_index = 0;

        // update scores from row
        for (int32_t j = 0; j < N; ++j)
        {
            int32_t score = FindIndex(numbers, table[i*N + j]);
            row_score = std::max(score, row_score);
            col_scores[col_index] = std::max(score, col_scores[col_index]);
            ++col_index;
        }

        // keep best score row
        bingo_score = std::min(bingo_score, row_score);
    }

    // compare best row score with col scores
    for (int32_t i = 0; i < N; ++i)
        bingo_score = std::min(bingo_score, col_scores[i]);

    return bingo_score;
}


int32_t ComputeTableScore(const TableType& table,
                         const std::vector<int32_t>& numbers,
                         int32_t max_index)
{
    int64_t sum = 0;
    for (int32_t v : table)
        sum += (int64_t)v;

    for (int32_t i = 0; i <= max_index; ++i)
        if (FindIndex(table, numbers[i]) >= 0)
            sum -= (int64_t)numbers[i];

    return static_cast<int32_t>(sum) * numbers[max_index];
}


int32_t day4_part1(const char* input_path)
{
    std::string line;
    int32_t best_table_bingo;
    TableType best_table = {};
    std::vector<int32_t> numbers;
    numbers.reserve(1024);

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        // get bingo numbers
        {
            std::getline(input_file, line);
            std::istringstream iss(line);
            uint32_t res;
            char comma;
            while (iss >> res)
            {
                numbers.push_back(res);
                //std::cout << res << '\n';
                iss >> comma;
            }
        }
        best_table_bingo = static_cast<int32_t>(numbers.size());

        // get a score for each array and keep track of the lower score
        // score = earliest that array will finish the bingo
        while (std::getline(input_file, line))
        {
            TableType table = {};
            if (ReadTable(input_file, table))
            {
                int32_t bingo_score = GetEarliestBingo(table, numbers);
                //std::cout << bingo_score << "\n";

                if (bingo_score < best_table_bingo)
                {
                    best_table_bingo = bingo_score;
                    best_table = table;
                }
            }
        }

        input_file.close();
    }

//     std::cout << best_table_bingo << "\n";
//     for (int32_t v : best_table)
//         std::cout << v << ", ";
//     std::cout<< "\n";

    int32_t score = ComputeTableScore(best_table, numbers, best_table_bingo);

    return score;
}

int32_t day4_part2(const char* input_path)
{
    std::string line;
    int32_t worst_table_bingo;
    TableType worst_table = {};
    std::vector<int32_t> numbers;
    numbers.reserve(1024);

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        // get bingo numbers
        {
            std::getline(input_file, line);
            std::istringstream iss(line);
            uint32_t res;
            char comma;
            while (iss >> res)
            {
                numbers.push_back(res);
                //std::cout << res << '\n';
                iss >> comma;
            }
        }
        worst_table_bingo = 0;

        // get a score for each array and keep track of the lower score
        // score = earliest that array will finish the bingo
        while (std::getline(input_file, line))
        {
            TableType table = {};
            if (ReadTable(input_file, table))
            {
                int32_t bingo_score = GetEarliestBingo(table, numbers);
                //std::cout << bingo_score << "\n";

                if (bingo_score > worst_table_bingo)
                {
                    worst_table_bingo = bingo_score;
                    worst_table = table;
                }
            }
        }

        input_file.close();
    }

//     std::cout << worst_table_bingo << "\n";
//     for (int32_t v : worst_table)
//         std::cout << v << ", ";
//     std::cout << "\n";

    int32_t score = ComputeTableScore(worst_table, numbers, worst_table_bingo);

    return score;
}
