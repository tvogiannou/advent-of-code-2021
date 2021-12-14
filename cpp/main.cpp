#include <cstdlib>
#include <cstdint>
#include <iostream>


// forward declarations
int32_t day1_part1(const char* input_path);
int32_t day1_part2(const char* input_path);
int32_t day2_part1(const char* input_path);
int32_t day2_part2(const char* input_path);
int32_t day3_part1(const char* input_path);
int32_t day3_part2(const char* input_path);


int main()
{
    int32_t result = -1;
//     result = day1_part1("D:/AoC/puzzles/inputs/day1_input.txt");
//     result = day1_part2("D:/AoC/puzzles/inputs/day1_input.txt");
//     result = day2_part1("D:/AoC/puzzles/inputs/day2_input.txt");
//     result = day2_part2("D:/AoC/puzzles/inputs/day2_input.txt");
//     result = day3_part1("D:/AoC/puzzles/inputs/day3_input.txt");
    result = day3_part2("D:/AoC/puzzles/inputs/day3_input.txt");

    std::cout << result << "\n";

    return EXIT_SUCCESS;
}