#ifndef MATRIX_ROTATION_H
#define MATRIX_ROTATION_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/matrix-rotation-algo
// there're a few better solutions on the site!
//
std::pair<int, int> rotate_left(int X, int Y, int row_s, int col_s, int row_e, int col_e, int steps)
{
    int edge_h = col_e - col_s;
    int edge_v = row_e - row_s;
    int perm   = 2 * (edge_h + edge_v);
    steps %= perm;
    int newX                 = X;
    int newY                 = Y;
    int distance_leftbottom  = row_e - X;
    int distance_rightbottom = distance_leftbottom + edge_h;
    int distance_righttop    = distance_rightbottom + edge_v;
    int distance_lefttop     = distance_righttop + edge_h;
    if (steps <= distance_leftbottom) {
        newX = X + steps;
    } else if (steps <= distance_rightbottom) {
        newX = row_e;
        newY = col_s + (steps - distance_leftbottom);
    } else if (steps <= distance_righttop) {
        newX = row_e - (steps - distance_rightbottom);
        newY = col_e;
    } else if (steps <= distance_lefttop) {
        newX = row_s;
        newY = col_e - (steps - distance_righttop);
    } else {
        newX = row_s + (steps - distance_lefttop);
        // newX = X - (steps - distance_lefttop);
    }
    return std::pair<int, int>(newX, newY);
}

std::pair<int, int> rotate_bottom(int X, int Y, int row_s, int col_s, int row_e, int col_e, int steps)
{
    int edge_h = col_e - col_s;
    int edge_v = row_e - row_s;
    int perm   = 2 * (edge_h + edge_v);
    steps %= perm;
    int newX                 = X;
    int newY                 = Y;
    int distance_rightbottom = col_e - Y;
    int distance_righttop    = distance_rightbottom + edge_v;
    int distance_lefttop     = distance_righttop + edge_h;
    int distance_leftbottom  = distance_lefttop + edge_v;
    if (steps <= distance_rightbottom) {
        newY = Y + steps;
    } else if (steps <= distance_righttop) {
        newX = row_e - (steps - distance_rightbottom);
        newY = col_e;
    } else if (steps <= distance_lefttop) {
        newX = row_s;
        newY = col_e - (steps - distance_righttop);
    } else if (steps <= distance_leftbottom) {
        newX = row_s + (steps - distance_lefttop);
        newY = col_s;
    } else {
        newY = col_s + (steps - distance_leftbottom);
        // newY = Y - (steps - distance_leftbottom);
    }
    return std::pair<int, int>(newX, newY);
}

std::pair<int, int> rotate_right(int X, int Y, int row_s, int col_s, int row_e, int col_e, int steps)
{
    int edge_h = col_e - col_s;
    int edge_v = row_e - row_s;
    int perm   = 2 * (edge_h + edge_v);
    steps %= perm;
    int newX                 = X;
    int newY                 = Y;
    int distance_righttop    = X - row_s;
    int distance_lefttop     = distance_righttop + edge_h;
    int distance_leftbottom  = distance_lefttop + edge_v;
    int distance_rightbottom = distance_leftbottom + edge_h;
    if (steps <= distance_righttop) {
        newX = X - steps;
    } else if (steps <= distance_lefttop) {
        newX = row_s;
        newY = col_e - (steps - distance_righttop);
    } else if (steps <= distance_leftbottom) {
        newX = row_s + (steps - distance_lefttop);
        newY = col_s;
    } else if (steps <= distance_rightbottom) {
        newX = row_e;
        newY = col_s + (steps - distance_leftbottom);
    } else {
        newX = row_e - (steps - distance_rightbottom);
        // newX = X + (steps - distance_rightbottom);
    }
    return std::pair<int, int>(newX, newY);
}

std::pair<int, int> rotate_top(int X, int Y, int row_s, int col_s, int row_e, int col_e, int steps)
{
    int edge_h = col_e - col_s;
    int edge_v = row_e - row_s;
    int perm   = 2 * (edge_h + edge_v);
    steps %= perm;
    int newX                 = X;
    int newY                 = Y;
    int distance_lefttop     = Y - col_s;
    int distance_leftbottom  = distance_lefttop + edge_v;
    int distance_rightbottom = distance_leftbottom + edge_h;
    int distance_righttop    = distance_rightbottom + edge_v;
    if (steps <= distance_lefttop) {
        newY = Y - steps;
    } else if (steps <= distance_leftbottom) {
        newX = row_s + (steps - distance_lefttop);
        newY = col_s;
    } else if (steps <= distance_rightbottom) {
        newX = row_e;
        newY = col_s + (steps - distance_leftbottom);
    } else if (steps <= distance_righttop) {
        newX = row_e - (steps - distance_rightbottom);
        newY = col_e;
    } else {
        newY = col_e - (steps - distance_righttop);
        // newY = Y + (steps - distance_righttop);
    }
    return std::pair<int, int>(newX, newY);
}

void rotateLayer(
    const std::vector<std::vector<int>>& matrix, int R, int row_s, int col_s, int row_e, int col_e,
    std::vector<std::vector<int>>& output)
{
    /*
     * ##############
     * #            #
     * #            #
     * #            #
     * #            #
     * ##############
     * */
    int X, Y;
    for (int i = row_s; i <= row_e; ++i) {
        // rotate elements on left col
        X                               = i;
        Y                               = col_s;
        auto pos1                       = rotate_left(X, Y, row_s, col_s, row_e, col_e, R);
        output[pos1.first][pos1.second] = matrix[X][Y];

        // rotate elements on right col
        X                               = i;
        Y                               = col_e;
        auto pos2                       = rotate_right(X, Y, row_s, col_s, row_e, col_e, R);
        output[pos2.first][pos2.second] = matrix[X][Y];
    }
    for (int i = col_s; i < col_e; ++i) {
        // rotate elements on bottom row
        X                               = row_e;
        Y                               = i;
        auto pos1                       = rotate_bottom(X, Y, row_s, col_s, row_e, col_e, R);
        output[pos1.first][pos1.second] = matrix[X][Y];

        // rotate elements on top row
        X                               = row_s;
        Y                               = i;
        auto pos2                       = rotate_top(X, Y, row_s, col_s, row_e, col_e, R);
        output[pos2.first][pos2.second] = matrix[X][Y];
    }
}

std::vector<std::vector<int>> matrix_rotation(const std::vector<std::vector<int>>& matrix, int R)
{
    int                           rows   = matrix.size();
    int                           cols   = matrix[0].size();
    int                           layers = ceil(std::min(rows, cols) / 2);
    std::vector<std::vector<int>> output(matrix);
    for (int layer = 0; layer < layers; ++layer) {
        int row_s = layer;
        int col_s = layer;
        int row_e = rows - layer - 1;
        int col_e = cols - layer - 1;
        rotateLayer(matrix, R, row_s, col_s, row_e, col_e, output);
    }
    return output;
}

#endif

