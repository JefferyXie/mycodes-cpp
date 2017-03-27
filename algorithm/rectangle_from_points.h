#ifndef RECTANGLE_FROM_POINTS_H
#define RECTANGLE_FROM_POINTS_H

#include "../main/header.h"

// 
/*
 * https://www.quora.com/From-a-given-set-of-points-how-would-you-find-the-set-of-4-points-forming-a-rectangle
 * Given a set of points, find the set of 4 points forming a rectangle
 * 
 * Examples :
 * 
 * Input : arr[] = TBD
 * Output : TODO
 * 
 */

// 1) Calculate the distances for each pair of points in the list, excluding repetitions (a,b) === (b,a)
// 2) Store the computed distance in an array an object with the distance and the two points involved
// 3) Sort the array by distance
// 4) Loop the array. For same distance (adjacent elements), the 4 points will form a rectangle if the 2 pairs
//    of points intersect at the center.
// To compute the center of a vector (a,b): center_x=(a_x+b_x)/2, center_y=(a_y+b_y)/2

int rectangle_from_points(int arr[], int n)
{
    // TODO...
    return 0;
}

#endif

