#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>


struct LineSegment
{
    int32_t x1, y1, x2, y2;
};

bool IsAxisParallel(const LineSegment& s)
{
    return (s.x1 == s.x2) || (s.y1 == s.y2);
}

uint64_t HashPoint(int32_t x1, int32_t y1)
{
    return ((uint64_t)x1) | ((uint64_t)y1 << 32);
}

int32_t day5_part1(const char* input_path)
{
    std::string line;
    std::vector<LineSegment> segments;
    segments.reserve(1024);

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            LineSegment s;
            char delim;
            iss >> s.x1 >> delim >> s.y1 >> delim >> delim >> s.x2 >> delim >> s.y2;
            //std::cout << x1 << "," << y1 << " -> " << x2 << "," << y2 << "\n";

            if (IsAxisParallel(s))
                segments.emplace_back(s);
        }
        input_file.close();
    }

    std::unordered_multiset<uint64_t> passed_points;
    for (const LineSegment& s : segments)
    {
        int32_t dx = 0, dy = 0;
        if (s.x1 == s.x2)
            dy = 1;
        else
            dx = 1;

        int32_t x = std::min(s.x1, s.x2);
        int32_t y = std::min(s.y1, s.y2);

        while ((x <= std::max(s.x1, s.x2)) &&
            (y <= std::max(s.y1, s.y2)))
        {
            uint64_t h = HashPoint(x, y);
            passed_points.emplace(h);
            x += dx;
            y += dy;
        }
    }

    int32_t count = 0;
    for (auto it = passed_points.begin(); it != passed_points.end(); )
    {
        int32_t _segment_count = passed_points.count(*it);
        if (_segment_count > 1)
            ++count;
        
        std::advance(it, _segment_count);
    }

    return count;
}


int32_t day5_part2(const char* input_path)
{
    std::string line;
    std::vector<LineSegment> segments;
    segments.reserve(1024);

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            LineSegment s;
            char delim;
            iss >> s.x1 >> delim >> s.y1 >> delim >> delim >> s.x2 >> delim >> s.y2;
            //std::cout << x1 << "," << y1 << " -> " << x2 << "," << y2 << "\n";

            segments.emplace_back(s);
        }
        input_file.close();
    }

    std::unordered_multiset<uint64_t> passed_points;
    for (const LineSegment& s : segments)
    {
        int32_t dx = 0, dy = 0;
        if (s.x1 < s.x2) dx = 1;
        else if (s.x1 > s.x2) dx = -1;
        if (s.y1 < s.y2) dy = 1;
        else if (s.y1 > s.y2) dy = -1;

        int32_t x = s.x1;
        int32_t y = s.y1;

        while (!((x == s.x2) && (y == s.y2)))
        {
            uint64_t h = HashPoint(x, y);
            passed_points.emplace(h);
            x += dx;
            y += dy;
        }
        passed_points.emplace(HashPoint(s.x2, s.y2));
    }

    int32_t count = 0;
    for (auto it = passed_points.begin(); it != passed_points.end(); )
    {
        int32_t _segment_count = passed_points.count(*it);
        if (_segment_count > 1)
            ++count;

        std::advance(it, _segment_count);
    }

    return count;
}
