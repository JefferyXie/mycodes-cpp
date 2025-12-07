#pragma once

#include "../core/header.h"

//
// Big endian and little endian are to handle multi-bytes words such as int, short, etc. C-style string is array of char
// which is one byte so endianness doesn't make sense for string.
//
// http://stackoverflow.com/questions/2247736/little-endian-vs-big-endian
// http://stackoverflow.com/questions/21478765/endian-representation-of-64-bit-values
// https://en.wikipedia.org/wiki/Endianness
// https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Data/endian.html
//
void run_endianness()
{
    std::cout << "run_endianness--------------" << std::endl;

    std::cout << "INT_MAX=" << INT_MAX << ", INT_MIN=" << INT_MIN << std::endl;
    std::cout << "pow(2, 31)=" << std::pow(2, 31) << std::endl;

    std::array<int, 3> arr = {{10, 10000, 1000000000}};
    std::for_each(std::begin(arr), std::end(arr), [](int i) {
        short s = (short)i;

        // this gives same result as 's' on little endian
        short s_LittleEndian = *(short*)&i;
        // this gives same result as 's' on big endian
        short s_BigEndian = *(((short*)&i) + 1);

        std::cout << "i=" << i << ", s=" << s << ", s_LittleEndian=" << s_LittleEndian
                  << ", s_BigEndian=" << s_BigEndian << std::endl;
    });
};

// http://www.geeksforgeeks.org/little-and-big-endian-mystery/
//
// function to show bytes in memory, from location start to start+n
void show_mem_rep(char* start, int n)
{
    for (int i = 0; i < n; i++)
        printf("| %0.2x | ---> %p \n", start[i], &(start[i]));
    printf("\n");
}
void checkEndianness()
{
    unsigned int i = 1;
    char*        c = (char*)&i;
    if (*c)
        printf("Little endian");
    else
        printf("Big endian");
}
void Run_show_mem_rep()
{
    checkEndianness();
    int i = 0x01234567;
    printf("\ni = %04x\n", i);
    show_mem_rep((char*)&i, sizeof(i));
}

//
// http://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func
//
// Byte swap unsigned short
uint16_t swap_uint16(uint16_t val)
{
    return (val << 8) | (val >> 8);
}

// Byte swap short
int16_t swap_int16(int16_t val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

// Byte swap unsigned int
uint32_t swap_uint32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

// Byte swap int
int32_t swap_int32(int32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | ((val >> 16) & 0xFFFF);
}
int64_t swap_int64(int64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

uint64_t swap_uint64(uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | (val >> 32);
}

//
// In-place swapping of bytes to match endianness of hardware
//
// @param[in/out] *object : memory to swap in-place
// @param[in]     _size   : length in bytes
//
void swap_bytes(void* _object, size_t _size)
{
    unsigned char *start, *end;

    for (start = (unsigned char*)_object, end = start + _size - 1; start < end; ++start, --end) {
        unsigned char swap = *start;
        *start             = *end;
        *end               = swap;
    }
}

#define SWAP_BYTES(x) swap_bytes(&x, sizeof(x));
