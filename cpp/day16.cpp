#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <bitset>
#include <functional>
#include <algorithm>


// bit mask for active bits in a byte from MSB to LSB
const uint8_t BYTE_MASK[8] = { 0x01, 0x03, 0x07,  0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

using OpHandler = std::function<uint64_t(uint64_t, uint64_t)>;
const OpHandler OP_DISPATCHER[8] =
{
    [](uint64_t a, uint64_t b) { return a + b; },               // sum
    [](uint64_t a, uint64_t b) { return a * b; },               // product
    [](uint64_t a, uint64_t b) { return std::min(a, b); },      // min
    [](uint64_t a, uint64_t b) { return std::max(a, b); },      // max
    [](uint64_t a, uint64_t b) { return 0u; },                  // null
    [](uint64_t a, uint64_t b) { return a > b ? 1u : 0u; },     // greater than
    [](uint64_t a, uint64_t b) { return a < b ? 1u : 0u; },     // less than
    [](uint64_t a, uint64_t b) { return a == b ? 1u : 0u; },    // equal
};

// naive bit stream
struct BitStream
{
    std::vector<uint8_t> bytes;
    int8_t bit_pos;
    int32_t byte_index;
    uint32_t bits_read;

    void Reset()
    {
        bits_read = 0;
        bit_pos = 7;
        byte_index = 0;
    }

    bool Ended() const
    {
        return byte_index == bytes.size();
    }

    uint32_t GetBits(int32_t count)
    {
        assert(count <= 32);
        bits_read += count;

        uint8_t mask = BYTE_MASK[bit_pos];
        uint32_t out = 0;
        out |= bytes[byte_index++] & mask;
        count -= bit_pos + 1;
        //out << bit_pos + 1;

        // write full bytes if any
        while (count >= 8)
        {
            out <<= 8;
            out |= bytes[byte_index++];
            count -= 8;
        }

        // resolve last part
        if (count > 0)
        {
            // need to add some extra bits from the current byte LSBs
            bit_pos = 8 - count - 1;
            //mask = ~BYTE_MASK[bit_pos];
            uint32_t val = bytes[byte_index];// &mask;
            val >>= bit_pos + 1;

            out <<= count;
            out |= val;
        }
        if (count < 0)
        {
            // need to remove some bits added
            --byte_index;
            bit_pos = -count - 1;
            out >>= -count;
        }
        if (count == 0)
            bit_pos = 7;

        return out;
    }
};

void ReadBytesFromFile(const char* input_path, BitStream& stream)
{
    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        std::getline(input_file, line);
        {
            std::istringstream iss(line);
            char c[2];
            while (iss >> c[0] >> c[1])
            {
                int32_t val;
                std::istringstream(std::string(c, 2)) >> std::hex >> val;
                //std::cout << std::hex << (int32_t)(val & 0xFF) << "\n";
                stream.bytes.emplace_back(static_cast<uint8_t>(val & 0xFF));
            }
        }
        input_file.close();
    }
}


struct PacketReader
{
    uint32_t version_sum = 0;

    struct PacketHeader
    {
        int32_t version;
        int32_t ID;
    };

    PacketHeader ReadHeader(BitStream& stream)
    {
        PacketHeader header;
        header.version = stream.GetBits(3);
        header.ID = stream.GetBits(3);

        return header;
    }

    uint64_t ReadLiteral(BitStream& stream)
    {
        uint64_t val = 0;
        while (stream.GetBits(1))
        {
            val <<= 4;
            val |= stream.GetBits(4);
        }
        val <<= 4;
        val |= stream.GetBits(4);

        return val;
    }

    uint64_t ReadOperator(BitStream& stream, const PacketHeader& h)
    {
        uint64_t res;
        auto op_handler = OP_DISPATCHER[h.ID];

        uint32_t length_type_ID = stream.GetBits(1);
        if (length_type_ID == 0)
        {
            int64_t total_length_bits = stream.GetBits(15);
            int64_t start_bits_read = stream.bits_read;
            res = ReadPacket(stream);
            while (stream.bits_read - start_bits_read < total_length_bits)
                res = op_handler(res, ReadPacket(stream));
        }
        else
        {
            uint32_t number_packets = stream.GetBits(11);
            res = ReadPacket(stream);
            for (uint32_t i = 1; i < number_packets; ++i)
                res = op_handler(res, ReadPacket(stream));
        }

        return res;
    }

    uint64_t ReadPacket(BitStream& stream)
    {
        PacketHeader h = ReadHeader(stream);
        version_sum += h.version;

        if (h.ID == 4)
            return ReadLiteral(stream);
        else
            return ReadOperator(stream, h);
    }
};


int32_t day16_part1(const char* input_path)
{
    BitStream stream;
    stream.bytes.reserve(1024);
    ReadBytesFromFile(input_path, stream);

//     stream.Reset();
//     std::cout << stream.GetBits(4) << "\n";
//     stream.Reset();
//     std::cout << stream.GetBits(12) << "\n";
//     stream.Reset();
//     std::cout << stream.GetBits(19) << "\n";
//     stream.Reset();
//     std::cout << stream.GetBits(24) << "\n";

    stream.Reset();
    PacketReader reader;
    reader.ReadPacket(stream);

    return reader.version_sum;
}


uint64_t day16_part2(const char* input_path)
{
    BitStream stream;
    stream.bytes.reserve(1024);
    ReadBytesFromFile(input_path, stream);

    stream.Reset();
    PacketReader reader;
    uint64_t res = reader.ReadPacket(stream);

    return res;
}
