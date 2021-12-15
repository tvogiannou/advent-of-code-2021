#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>



uint64_t AdvanceDay(std::vector<uint64_t>& timers)
{
    uint64_t zeroed = timers[0];

    timers[0] = timers[1];
    timers[1] = timers[2];
    timers[2] = timers[3];
    timers[3] = timers[4];
    timers[4] = timers[5];
    timers[5] = timers[6];
    timers[6] = zeroed + timers[7];
    timers[7] = timers[8];
    timers[8] = zeroed;

    return zeroed;
}

uint64_t day6_part1(const char* input_path)
{
    const int32_t NUM_DAYS = 80;
    std::vector<uint64_t> timers = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint64_t count = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        std::getline(input_file, line);
        {
            std::istringstream iss(line);
            int32_t val;
            char delim;
            while (iss >> val)
            {
                timers[val] += 1;
                ++count;

                iss >> delim;
            }
        }
        input_file.close();
    }

    for (int32_t i = 0; i < NUM_DAYS; ++i)
        count += AdvanceDay(timers);

    return count;
}


uint64_t day6_part2(const char* input_path)
{
    const int32_t NUM_DAYS = 256;
    std::vector<uint64_t> timers = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint64_t count = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        std::getline(input_file, line);
        {
            std::istringstream iss(line);
            int32_t val;
            char delim;
            while (iss >> val)
            {
                timers[val] += 1;
                ++count;

                iss >> delim;
            }
        }
        input_file.close();
    }

    for (int32_t i = 0; i < NUM_DAYS; ++i)
        count += AdvanceDay(timers);

    return count;
}
