#ifndef FIND_K_MOST_FREQUENT_WORDS_H
#define FIND_K_MOST_FREQUENT_WORDS_H

#include "../main/header.h"

// 
/*
 * http://stackoverflow.com/questions/185697/the-most-efficient-way-to-find-top-k-frequent-words-in-a-big-word-sequence
 * Find the top k most frequent words from a file or input text
 * 
 * 
 */

// Solution 1:
// 
// 1) Count words and hash it, which will end up in the structure like this
// var hash = {
//   "I" : 13,
//   "like" : 3,
//   "meow" : 3,
//   "geek" : 3,
//   "burger" : 2,
//   "cat" : 1,
//   "foo" : 100,
//   ...
//   ...
// 2) Traverse through the hash and find the most frequently used word (in this case "foo" 100),
//    then create the array of that size
// 3) Then we can traverse the hash again and use the number of occurrences of words as array
//    index, if there is nothing in the index, create an array else append it in the array. Then
//    we end up with an array like:
//     0   1      2            3                 100
//   [[ ],[ ],[burger],[like, meow, geek],[]...,[foo]]
//    Then just traverse the array from the end, and collect the k words.


// Solution 2:
// 
// 1) Same as above
// 2) Use min-heap (std::priority_queue) and keep the size of min heap to k, and for each word in
//    the hash we compare the occurrences of words with the min-heap - if it's greater than the
//    min value, remove the min (if the size of the min heap is equal to k) and insert the number
//    in the min-heap
// 3) After traversing through the array, just convert the min-heap to array and return the array.


// Solution 3:
// Use trie and min-heap
// http://www.geeksforgeeks.org/find-the-k-most-frequent-words-from-a-file/

// TODO...

#endif

