#pragma once

#include "../core/header.h"

//
/*
 * Remove arbitrary spaces from a sentence
 *
 * Examples :
 * Input : "  The   sky is      blue  !    "
 * Output : "The sky is blue  !"
 *
 */

// change input's content with result, return the number of valid chars
int remove_arbitrary_space(char* input, int length)
{
    int count_valid_char = 0;
    for (int i = 0; i < length; ++i) {
        if (*(input + i) == ' ') {
            if (i == 0 || *(input + i - 1) == ' ')
                continue;
        }
        input[count_valid_char++] = *(input + i);
    }
    // remove the last space
    if (input[count_valid_char - 1] == ' ') {
        input[count_valid_char - 1] = '\0';
    }
    std::cout << "\"" << input << "\"" << std::endl;
    return count_valid_char;
}

// don't change input's content but write to result
int remove_arbitrary_space(char* input, int length, char* result)
{
    int count_valid_char = 0;
    for (int i = 0; i < length; ++i) {
        if (*(input + i) == ' ') {
            if (i == 0 || *(input + i - 1) == ' ')
                continue;
        }
        result[count_valid_char++] = *(input + i);
    }
    // remove the last space
    if (result[count_valid_char - 1] == ' ') {
        result[count_valid_char - 1] = '\0';
    }
    return count_valid_char;
}

