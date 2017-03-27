#ifndef SUBSTRING_LONGEST_UNIQUE_H
#define SUBSTRING_LONGEST_UNIQUE_H

#include "../main/header.h"

// 
// http://www.geeksforgeeks.org/length-of-the-longest-substring-without-repeating-characters/
// 
// Given a string, find the length of the longest substring without repeating characters. For 
// example, the longest substrings without repeating characters for “ABDEFGABEF” are “BDEFGA” 
// and “DEFGAB”, with length 6. For “BBBB” the longest substring is “B”, with length 1. For 
// “GEEKSFORGEEKS”, there are two longest substrings shown in the below diagrams, with length 7.

// solution 1):
// http://stackoverflow.com/questions/9734474/find-longest-substring-without-repeating-characters
// 
// 1) You are going to need a start and an end locator(/pointer) for the string and an array where
//    you store information for each character: did it occour at least once?
// 2) Start at the beginning of the string, both locators point to the start of the string.
// 3) Move the end locator to the right till you find a repetition (or reach the end of the string).
//    For each processed character, store it in the array. When stopped store the position if this is
//    the largest substring. Also remember the repeated character.
// 4) Now do the same thing with the start locator, when processing each character, remove its flags
//    from the array. Move the locator till you find the repeated character.
// 5) Go back to step 3 if you haven't reached the end of string.
// Overall: O(N)

// solution 2):
// The solution 1) uses two locators and runs O(2*N). We can save character's index information into
// the array so just use one locator and runs exactly O(N).
std::string substring_longest_unique(char *str) {
    int visited[26] = {0};  // 'a'--'z'
 
    /* Initialize the visited array as -1, -1 is used to
       indicate that character has not been visited yet. */
    for (int i = 0; i < (int)(sizeof(visited)/sizeof(visited[0]));  i++)
    {
        visited[i] = -1;
    }
 
    /* Mark first character as visited by storing the index
       of first   character in visited array. */
    visited[str[0]-'a'] = 0;
 
    int cur_len = 1;        // length of current substring
    int max_len = 1;        // result
    int prev_index;         // previous index
    int result_start = 0;   // index of result substring's starting position
    /* Start from the second character. First character is
       already processed (cur_len and max_len are initialized
       as 1, and visited[str[0]] is set */
    for (int i = 1; i < (int)strlen(str); i++)
    {
        prev_index = visited[str[i]-'a'];
 
        /* If the currentt character is not present in the
           already processed substring or it is not part of
           the current NRCS, then do cur_len++ */
        if (prev_index == -1 || i - cur_len > prev_index)
        {
            cur_len++;
        }
        /* If the current character is present in currently
           considered NRCS, then update NRCS to start from
           the next character of previous instance. */
        else
        {
            /* Also, when we are changing the NRCS, we
               should also check whether length of the
               previous NRCS was greater than max_len or
               not.*/
            if (cur_len > max_len)
            {
                max_len = cur_len;
                result_start = i - cur_len - 1;
            }
 
            cur_len = i - prev_index;
        }
 
        // update the index of current character
        visited[str[i]-'a'] = i;
    }
 
    // Compare the length of last NRCS with max_len and
    // update max_len if needed
    if (cur_len > max_len)
    {
        max_len = cur_len;
    }
 
    return std::string(str).substr(result_start, max_len);
}

// solution 3),4)
// https://leetcode.com/articles/longest-substring-without-repeating-characters/
// 

#endif

