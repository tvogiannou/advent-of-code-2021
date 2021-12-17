#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>


int32_t GetCharCost(char c)
{
    switch (c)
    {
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
    default:
        break;
    }

    return 0;
}

int32_t GetCharCost2(char c)
{
    switch (c)
    {
    case '(': return 1;
    case '[': return 2;
    case '{': return 3;
    case '<': return 4;
    default:
        break;
    }

    return 0;
}

bool SafeCheckChar(const std::vector<char>& chunks, char expected)
{
    if (!chunks.empty() && chunks.back() != expected)
        return false;
    return true;
}

bool CheckChunk(std::vector<char>& chunks, char c)
{
    switch (c)
    {
    case '(':
    case '[':
    case '{':
    case '<':
        { 
            chunks.emplace_back(c);
            break;
        }
    case ')':
        {
            if (!SafeCheckChar(chunks, '(')) 
                return false;
            chunks.pop_back();
            break;
        }
    case ']':
        {
            if (!SafeCheckChar(chunks, '['))
                return false;
            chunks.pop_back();
            break;
        }
    case '}':
        {
            if (!SafeCheckChar(chunks, '{'))
                return false;
            chunks.pop_back();
            break;
        }
    case '>': 
        {
            if (!SafeCheckChar(chunks, '<'))
                return false;
            chunks.pop_back();
            break;
        }
    default:
        break;
    }

    return true;
}

int64_t ComputeCompletionScore(std::vector<char>& chunks)
{
    int64_t score = 0;

    while (!chunks.empty())
    {
        char c = chunks.back();
        chunks.pop_back();
        score *= 5;
        score += GetCharCost2(c);
    }

    return score;
}

int32_t day10_part1(const char* input_path)
{
    std::vector<char> chunks;
    chunks.reserve(1024);
    int32_t error_score = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        while(std::getline(input_file, line))
        {
            chunks.clear();

            std::istringstream iss(line);
            char val;
            while (iss >> val)
            {
                if (!CheckChunk(chunks, val))
                {
                    error_score += GetCharCost(val);
                    break;
                }
            }
            //std::cout << error_score << "\n";
        }
        input_file.close();
    }

    return error_score;
}

int64_t day10_part2(const char* input_path)
{
    std::vector<char> chunks;
    chunks.reserve(1024);
    std::vector<int64_t> scores;
    scores.reserve(1024);

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            chunks.clear();
            bool corrupted = false;

            std::istringstream iss(line);
            char val;
            while (iss >> val)
            {
                if (!CheckChunk(chunks, val))
                {
                    corrupted = true;
                    break;
                }
            }

            if (!corrupted)
            {
                scores.emplace_back(ComputeCompletionScore(chunks));
                std::cout << scores.back() << "\n";
            }
        }
        input_file.close();
    }

    std::sort(scores.begin(), scores.end());

    int64_t mid_score = scores[scores.size() / 2];

    return mid_score;
}
