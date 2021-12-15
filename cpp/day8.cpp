#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <assert.h>


uint8_t CHAR_TO_BITS[128];
void SetupCharMap()
{
    CHAR_TO_BITS['a'] = 0x40;
    CHAR_TO_BITS['b'] = 0x20;
    CHAR_TO_BITS['c'] = 0x10;
    CHAR_TO_BITS['d'] = 0x08;
    CHAR_TO_BITS['e'] = 0x04;
    CHAR_TO_BITS['f'] = 0x02;
    CHAR_TO_BITS['g'] = 0x01;
}


uint8_t Encode(const std::string& str)
{
    uint8_t encoding = 0x00;
    for (char c : str)
        encoding |= CHAR_TO_BITS[c];

    return encoding;
}

int32_t CountBits(uint8_t c)
{
    int32_t count = 0;
    while (c)
    {
        if (0x01 & c) ++count;
        c >>= 1;
    }

    return count;
}

struct Stack3
{
    static const int32_t N = 3;
    
    uint8_t val[N];
    int32_t len = 0;

    void push(uint8_t v) { assert(len < N);  val[len++] = v; }
};


// ENCODIG MAP
// 0 | abcdef
// 1 | bc
// 2 | abdeg
// 3 | abcdg
// 4 | bcfg
// 5 | acdfg
// 6 | acdefg
// 7 | abc
// 8 | abcdefg
// 9 | abcdfg
using EncodingArray = std::array<uint8_t, 10>;
using DecodeMap = std::array<uint8_t, 256>;
void CreateDecoding(DecodeMap& decoder, const EncodingArray& encodings)
{
    decoder.fill(0x00);

    uint8_t digit_1 = 0x00;
    uint8_t digit_4 = 0x00;
    uint8_t digit_6 = 0x00;
    uint8_t digit_7 = 0x00;
    Stack3 len_5, len_6;

    // set known numbers based on length
    for (uint8_t c : encodings)
    {
        int32_t bit_count = CountBits(c);
        switch (bit_count)
        {
        case 2: { decoder[c] = 1; digit_1 = c; break; }
        case 3: { decoder[c] = 7; digit_7 = c; break; }
        case 4: { decoder[c] = 4; digit_4 = c; break; }
        case 5: { len_5.push(c); break; }
        case 6: { len_6.push(c); break; }
        case 7: { decoder[c] = 8; break; }
        default:
            // should never reach here
            assert(false);
            break;
        }
    }

    // first resolve the length 6 characters
    for (int32_t i=0; i<len_6.len; ++i)
    {
        uint8_t c = len_6.val[i];
        uint8_t c_1 = digit_1 & c;
        uint8_t c_4 = digit_4 & c;

        if (CountBits(c_1) == 1)
        {
            decoder[c] = 6;
            digit_6 = c;
            len_6.val[i] = 0x00;
        }
        if (CountBits(c_4) == 4)
        {
            len_6.val[i] = 0x00;
            decoder[c] = 9;
        }
    }

    for (int32_t i = 0; i < len_6.len; ++i)
    {
        uint8_t c = len_6.val[i];
        if (c) decoder[c] = 0;
    }

    // now resolve the length 5 characters
    for (int32_t i = 0; i < len_5.len; ++i)
    {
        uint8_t c = len_5.val[i];
        uint8_t c_6 = digit_6 & c;
        uint8_t c_7 = digit_7 & c;

        if (CountBits(c_6) == 5)
        {
            decoder[c] = 5;
            len_5.val[i] = 0x00;
        }
        if (CountBits(c_7) == 3)
        {
            len_5.val[i] = 0x00;
            decoder[c] = 3;
        }
    }

    for (int32_t i = 0; i < len_5.len; ++i)
    {
        uint8_t c = len_5.val[i];
        if (c) decoder[c] = 2;
    }
}


int32_t day8_part1(const char* input_path)
{
    SetupCharMap();

    int32_t count = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            bool right_side = false;
            std::istringstream iss(line);
            std::string val;
            while (iss >> val)
            {
                if (right_side)
                {
                    uint8_t encoding = Encode(val);
                    int32_t bit_count = CountBits(encoding);

                    switch (bit_count)
                    {
                    case 2: // no 1
                    case 4: // no 4
                    case 3: // no 7
                    case 7: // no 8
                        ++count;
                        break;
                    default:
                        break;
                    }
                }

                if (val.compare("|") == 0)
                    right_side = true;
            }
        }
        input_file.close();
    }

    return count;
}


int32_t day8_part2(const char* input_path)
{
    SetupCharMap();

    int32_t count = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            EncodingArray encodings;
            int32_t encodings_index = 0;
            DecodeMap decoder;
            bool right_side = false;
            int32_t multiplier = 1000;
            int32_t result = 0;

            std::istringstream iss(line);
            std::string val;
            while (iss >> val)
            {
                if (val.compare("|") == 0)
                {
                    right_side = true;
                    CreateDecoding(decoder, encodings);
                    continue;
                }

                uint8_t encoding = Encode(val);
                if (right_side)
                {
                    result += multiplier * static_cast<int32_t>(decoder[encoding]);
                    multiplier /= 10;
                }
                else
                    encodings[encodings_index++] = encoding;
            }

            count += result;
            //std::cout << result << "\n";
        }
        input_file.close();
    }

    return count;
}
