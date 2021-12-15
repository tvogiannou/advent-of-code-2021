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
int32_t day4_part1(const char* input_path);
int32_t day4_part2(const char* input_path);
int32_t day5_part1(const char* input_path);
int32_t day5_part2(const char* input_path);
uint64_t day6_part1(const char* input_path);
uint64_t day6_part2(const char* input_path);
int32_t day7_part1(const char* input_path);
int32_t day7_part2(const char* input_path);

// util to just print any return type
template<typename T>
void PrintResult(const char* testname, T result)
{
    std::cout << testname << ": " << result << "\n";
}


int main()
{
    PrintResult("D1 P1", day1_part1("D:/AoC/puzzles/inputs/day1_input.txt"));
    PrintResult("D1 P2", day1_part2("D:/AoC/puzzles/inputs/day1_input.txt"));
    PrintResult("D2 P1", day2_part1("D:/AoC/puzzles/inputs/day2_input.txt"));
    PrintResult("D2 P2", day2_part2("D:/AoC/puzzles/inputs/day2_input.txt"));
    PrintResult("D3 P1", day3_part1("D:/AoC/puzzles/inputs/day3_input.txt"));
    PrintResult("D3 P2", day3_part2("D:/AoC/puzzles/inputs/day3_input.txt"));
    PrintResult("D4 P1", day4_part1("D:/AoC/puzzles/inputs/day4_input.txt"));
    PrintResult("D4 P2", day4_part2("D:/AoC/puzzles/inputs/day4_input.txt"));
    PrintResult("D5 P1", day5_part1("D:/AoC/puzzles/inputs/day5_input.txt"));
    PrintResult("D5 P2", day5_part2("D:/AoC/puzzles/inputs/day5_input.txt"));
    PrintResult("D6 P1", day6_part1("D:/AoC/puzzles/inputs/day6_input.txt"));
    PrintResult("D6 P2", day6_part2("D:/AoC/puzzles/inputs/day6_input.txt"));
    PrintResult("D7 P1", day7_part1("D:/AoC/puzzles/inputs/day7_input.txt"));
    PrintResult("D7 P2", day7_part2("D:/AoC/puzzles/inputs/day7_input.txt"));

    return EXIT_SUCCESS;
}