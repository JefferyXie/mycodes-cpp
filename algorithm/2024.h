#pragma once

#include "../core/node.h"
#include "../core/util.h"

///////////////////////////////////////////////////////////////////////////////
//
// Find the game value by program:
// 1) given the number of red and green balls;
// 2) the price you win if you pick up red and green respectively;
// 3) the number of times you may choose;
//
// time complexity: O(2^min(times, n_red + n_green))
//
double calc_game_value(
    uint32_t times, int32_t n_red, int32_t n_green, int32_t win_red, int32_t win_green, uint32_t& call_times)
{
    if (times == 0)
        return 0;
    else if (n_red == 0)
        return win_green * n_green;
    else if (n_green == 0)
        return win_red * n_red;

    ++call_times;

    auto ratio     = (double)n_red / (n_red + n_green);
    auto value_red = ratio * (win_red + calc_game_value(times - 1, n_red - 1, n_green, win_red, win_green, call_times));
    auto value_green =
        (1 - ratio) * (win_green + calc_game_value(times - 1, n_red, n_green - 1, win_red, win_green, call_times));

    std::cout << "\tcalc_game_value: times=" << times << ", n_red=" << n_red << ", n_green=" << n_green
              << "; value_red=" << value_red << ", value_green=" << value_green << std::endl;

    return value_red + value_green;
}

// time complexity: O(min(times, n_red + n_green) ^ 2 / 2)
// why divided by 2? because we care about the counts (n_red & n_green) only, the order doesn't matter!
double calc_game_value_dp(
    uint32_t times, int32_t n_red, int32_t n_green, int32_t win_red, int32_t win_green, uint32_t& call_times,
    std::vector<std::vector<double>>& dp)
{
    auto valid = [](double v) {
        return v != std::numeric_limits<double>::min();
    };

    auto& result = dp[n_red][n_green];
    if (valid(result)) {
        return result;
    }

    if (times == 0) {
        result = 0;
    } else if (n_red == 0) {
        result = win_green * n_green;
    } else if (n_green == 0) {
        result = win_red * n_red;
    } else {
        ++call_times;

        const auto ratio = (double)n_red / (n_red + n_green);

        auto& value_red_1 = dp[n_red - 1][n_green];
        if (!valid(value_red_1)) {
            value_red_1 = calc_game_value_dp(times - 1, n_red - 1, n_green, win_red, win_green, call_times, dp);
        }
        const auto value_red = ratio * (win_red + value_red_1);

        auto& value_green_1 = dp[n_red][n_green - 1];
        if (!valid(value_green_1)) {
            value_green_1 = calc_game_value_dp(times - 1, n_red, n_green - 1, win_red, win_green, call_times, dp);
        }
        const auto value_green = (1 - ratio) * (win_green + value_green_1);

        result = value_red + value_green;

        std::cout << "\tcalc_game_value_dp: times=" << times << ", n_red=" << n_red << ", n_green=" << n_green
                  << "; value_red=" << value_red << ", value_green=" << value_green << ", result=" << result
                  << std::endl;
    }
    return result;
}

void run_calc_game_value(uint32_t times, int32_t n_red, int32_t n_green, int32_t win_red, int32_t win_green)
{
    std::cout << "\ncalc_game_value: times=" << times << ", n_red=" << n_red << ", n_green=" << n_green
              << ", win_red=" << win_red << ", win_green=" << win_green << std::endl;
    /*
        double value = 0;
        {
            uint32_t call_times = 0;
            value               = calc_game_value(times, n_red, n_green, win_red, win_green, call_times);
            std::cout << "Result=" << (value < 1e-9 ? 0 : value) << ", call_times=" << call_times << std::endl;
        }
    */
    double value_dp = 0;
    {
        uint32_t                         call_times = 0;
        std::vector<std::vector<double>> dp(
            n_red + 1, std::vector<double>(n_green + 1, std::numeric_limits<double>::min()));
        value_dp = calc_game_value_dp(times, n_red, n_green, win_red, win_green, call_times, dp);
        std::cout << "Result=" << (value_dp < 1e-9 ? 0 : value_dp) << ", call_times=" << call_times << std::endl;
    }
    /*
        if (std::fabs(value - value_dp) > 1e-9) {
            std::cout << "Don't match: value=" << value << ", value_dp=" << value_dp << std::endl;
        }
    */
}

void run_game_value()
{
    run_calc_game_value(9, 2, 0, 1, -1);
    run_calc_game_value(0, 2, 0, 1, -1);
    run_calc_game_value(1, 2, 0, 1, -1);
    run_calc_game_value(2, 2, 0, 1, -1);
    run_calc_game_value(3, 2, 0, 1, -1);
    run_calc_game_value(4, 2, 0, 1, -1);

    run_calc_game_value(9, 3, 3, 1, -1);
    run_calc_game_value(3, 3, 3, 1, -1);
    run_calc_game_value(4, 3, 3, 1, -1);
    run_calc_game_value(5, 3, 3, 1, -1);
    run_calc_game_value(9, 4, 3, 1, -1);

    run_calc_game_value(1, 26, 26, 1, -1);
    run_calc_game_value(2, 26, 26, 1, -1);
    run_calc_game_value(4, 26, 26, 1, -1);
    run_calc_game_value(8, 26, 26, 1, -1);
    run_calc_game_value(9, 26, 26, 1, -1);
    run_calc_game_value(10, 26, 26, 1, -1);
    run_calc_game_value(11, 26, 26, 1, -1);
    run_calc_game_value(12, 26, 26, 1, -1);

    run_calc_game_value(52, 26, 26, 1, -1);
    run_calc_game_value(50, 26, 26, 1, -1);
    run_calc_game_value(45, 26, 26, 1, -1);
    run_calc_game_value(25, 26, 26, 1, -1);
}

///////////////////////////////////////////////////////////////////////////////
//
// Given a (m * n) matrix, return all elements in spiral order
// https://leetcode.com/problems/spiral-matrix/
// https://leetcode.com/problems/spiral-matrix/solutions/3845051/unraveling-the-spiral-traverse-your-matrix-in-style/
//
std::vector<int> matrix_spiral_order(const std::vector<std::vector<int>>& matrix)
{
    std::vector<int> result;
    auto             left_right = [&](int c_start, int c_end, int r) {
        for (int c = c_start; c < c_end; ++c) {
            result.emplace_back(matrix[r][c]);
        }
    };
    auto top_bottom = [&](int r_start, int r_end, int c) {
        for (int r = r_start; r < r_end; ++r) {
            result.emplace_back(matrix[r][c]);
        }
    };
    auto right_left = [&](int c_start, int c_end, int r) {
        for (int c = c_start; c > c_end; --c) {
            result.emplace_back(matrix[r][c]);
        }
    };
    auto bottom_top = [&](int r_start, int r_end, int c) {
        for (int r = r_start; r > r_end; --r) {
            result.emplace_back(matrix[r][c]);
        }
    };

    const int rows = matrix.size();
    const int cols = matrix.front().size();
    for (int i = 0; i < (rows + 1) / 2; ++i) {
        // if last element is right in the middle (matrix is sqaure), just add it and done
        if (result.size() + 1 == (size_t)(rows * cols)) {
            result.emplace_back(matrix[i][i]);
            break;
        }

        {
            int c_start = i;
            int c_end   = cols - i - 1;
            int r       = i;
            left_right(c_start, c_end, r);
        }

        {
            int r_start = i;
            int r_end   = rows - i - 1;
            int c       = cols - i - 1;
            top_bottom(r_start, r_end, c);
        }

        {
            int c_start = cols - i - 1;
            int c_end   = i;
            int r       = rows - i - 1;
            right_left(c_start, c_end, r);
        }

        {
            int r_start = rows - i - 1;
            int r_end   = i;
            int c       = i;
            bottom_top(r_start, r_end, c);
        }
    }

    return result;
}

std::vector<int> matrix_spiral_order_v2(const std::vector<std::vector<int>>& matrix)
{
    constexpr std::array<std::array<int, 2>, 4> rotations = {{
        {0, 1},    // {r, c}
        {1, 0},
        {0, -1},
        {-1, 0},
    }};

    std::vector<int> result;
    const int        rows = matrix.size();
    const int        cols = matrix.front().size();
    for (int i = 0; i < (rows + 1) / 2; ++i) {
        int r = i;
        int c = i;

        // if last element is right in the middle (matrix is sqaure), just add it and done
        if (result.size() + 1 == (size_t)(rows * cols)) {
            result.emplace_back(matrix[r][c]);
            break;
        }

        for (const auto& rotation : rotations) {
            // 1) for each rotation, add the starting point but leave the last to next rotation
            // 2) the most tricky part is the boundary of 'r' and 'c' that depend on 'rotation' in use
            const int min_r = i - rotation[0];
            const int max_r = rows - i - rotation[0];
            const int min_c = i - rotation[1];
            const int max_c = cols - i - rotation[1];
            while ((r >= min_r && r < max_r) && (c >= min_c && c < max_c)) {
                result.emplace_back(matrix[r][c]);
                r += rotation[0];
                c += rotation[1];
            }
        }
    }

    return result;
}

std::vector<int> matrix_spiral_order_v3(const std::vector<std::vector<int>>& matrix)
{
    constexpr std::array<std::array<int, 2>, 4> rotations = {{
        {0, 1},    // {r, c}
        {1, 0},
        {0, -1},
        {-1, 0},
    }};

    std::vector<int> result;
    const int        rows    = matrix.size();
    const int        cols    = matrix.front().size();
    int              start_x = 0;
    int              start_y = 0;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    visited[0][0] = true;
    result.emplace_back(matrix[0][0]);    // add first element

    while (result.size() < (size_t)rows * cols) {
        for (const auto& rotation : rotations) {
            auto r = start_x + rotation[0];
            auto c = start_y + rotation[1];
            while ((r >= 0 && r < rows) && (c >= 0 && c < cols) && !visited[r][c]) {
                result.emplace_back(matrix[r][c]);
                visited[r][c] = true;

                start_x = r;
                start_y = c;

                r += rotation[0];
                c += rotation[1];
            }
        }
    }

    return result;
}

// Given a (m * n) matrix, return all elements in diagonal order
// https://leetcode.com/problems/diagonal-traverse-ii/description/
std::vector<int> matrix_diagonal_order(const std::vector<std::vector<int>>& matrix)
{
    std::vector<int> result;
    const size_t     rows = matrix.size();
    size_t           cols = 0;
    std::for_each(matrix.begin(), matrix.end(), [&](auto& v) {
        cols = std::max(cols, v.size());
    });
    for (size_t i = 0; i < rows + cols - 1; ++i) {
        // adjusted row as boundary and starting point
        const int r_adj = std::min(i, rows - 1);
        for (size_t j = i - r_adj; j <= i && j < cols; ++j) {
            // given same cycle/round, (r + c == i)
            size_t r = i - j;
            size_t c = j;
            if (matrix[r].size() > c) {
                result.push_back(matrix[r][c]);
            }
        }
    }
    return result;
}

// Given a (m * n) matrix, return all elements in diagonal-rotation-direction order
// https://leetcode.com/problems/diagonal-traverse/description/
std::vector<int> matrix_diagonal_rotation_order(const std::vector<std::vector<int>>& matrix)
{
    std::vector<int> result;
    const size_t     rows = matrix.size();
    size_t           cols = 0;
    std::for_each(matrix.begin(), matrix.end(), [&](auto& v) {
        cols = std::max(cols, v.size());
    });
    for (size_t i = 0; i < rows + cols - 1; ++i) {
        // switch traverse direction, need the adjusted row/col for starting point at each round
        if (i % 2 == 0) {
            // adjusted row as boundary and starting point
            const int r_adj = std::min(i, rows - 1);
            for (size_t j = i - r_adj; j <= i && j < cols; ++j) {
                // given same cycle/round, (r + c == i)
                size_t r = i - j;
                size_t c = j;
                if (matrix[r].size() > c) {
                    result.push_back(matrix[r][c]);
                }
            }
        } else {
            // adjusted col as boundary and starting point
            int c_adj = std::min(i, cols - 1);
            for (size_t j = i - c_adj; j <= i && j < rows; ++j) {
                // given same cycle/round, (r + c == i)
                size_t r = j;
                size_t c = i - j;
                if (matrix[r].size() > c) {
                    result.push_back(matrix[r][c]);
                }
            }
        }
    }
    return result;
}

void run_matrix_diagonal_order()
{
    using use_case_t = std::pair<std::vector<std::vector<int>>, std::vector<int>>;
    for (auto [matrix, exp_v] : {
             use_case_t{{{1}}, {1}},
             use_case_t{{{1, 2}}, {1, 2}},
             use_case_t{{{1}, {2}}, {1, 2}},
             use_case_t{{{1, 2}, {3, 4}}, {1, 3, 2, 4}},
             use_case_t{
                 {
                     {1, 2, 3},
                     {4, 5, 6},
                     {7, 8, 9},
                 },
                 {1, 4, 2, 7, 5, 3, 8, 6, 9}},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {5, 6, 7, 8},
                     {9, 10, 11, 12},
                 },
                 {1, 5, 2, 9, 6, 3, 10, 7, 4, 11, 8, 12}},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {5, 6, 7, 8},
                     {9, 10, 11, 12},
                     {13, 14, 15, 16},
                 },
                 {1, 5, 2, 9, 6, 3, 13, 10, 7, 4, 14, 11, 8, 15, 12, 16}},
             use_case_t{
                 {
                     {1, 2, 3, 4, 5},
                     {6, 7},
                     {8},
                     {9, 10, 11},
                     {12, 13, 14, 15, 16},
                 },
                 {1, 6, 2, 8, 7, 3, 9, 4, 12, 10, 5, 13, 11, 14, 15, 16}},
         }) {
        const auto v = matrix_diagonal_order(matrix);
        std::cout << __FUNCTION__ << ": matrix=" << util::dump_matrix(matrix, true)
                  << ", matrix_diagonal_order=" << util::dump_array(v) << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

void run_matrix_diagonal_rotation_order()
{
    using use_case_t = std::pair<std::vector<std::vector<int>>, std::vector<int>>;
    for (auto [matrix, exp_v] : {
             use_case_t{{{1}}, {1}},
             use_case_t{{{1, 2}}, {1, 2}},
             use_case_t{{{1}, {2}}, {1, 2}},
             use_case_t{{{1, 2}, {3, 4}}, {1, 2, 3, 4}},
             use_case_t{
                 {
                     {1, 2, 3},
                     {4, 5, 6},
                     {7, 8, 9},
                 },
                 {1, 2, 4, 7, 5, 3, 6, 8, 9}},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {5, 6, 7, 8},
                     {9, 10, 11, 12},
                 },
                 {1, 2, 5, 9, 6, 3, 4, 7, 10, 11, 8, 12}},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {5, 6, 7, 8},
                     {9, 10, 11, 12},
                     {13, 14, 15, 16},
                 },
                 {1, 2, 5, 9, 6, 3, 4, 7, 10, 13, 14, 11, 8, 12, 15, 16}},
             use_case_t{
                 {
                     {1, 2, 3, 4, 5},
                     {6, 7},
                     {8},
                     {9, 10, 11},
                     {12, 13, 14, 15, 16},
                 },
                 {1, 2, 6, 8, 7, 3, 4, 9, 12, 10, 5, 11, 13, 14, 15, 16}},
         }) {
        const auto v = matrix_diagonal_rotation_order(matrix);
        std::cout << __FUNCTION__ << ": matrix=" << util::dump_matrix(matrix, true)
                  << ", matrix_diagonal_rotation_order=" << util::dump_array(v) << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

void run_matrix_spiral_order()
{
    using use_case_t = std::pair<std::vector<std::vector<int>>, std::vector<int>>;
    for (auto [matrix, exp_v] : {
             use_case_t{{{1}}, {1}},
             use_case_t{{{1, 2}}, {1, 2}},
             use_case_t{{{1}, {2}}, {1, 2}},
             use_case_t{{{1, 2}, {3, 4}}, {1, 2, 4, 3}},
             use_case_t{
                 {
                     {1, 2, 3},
                     {4, 5, 6},
                     {7, 8, 9},
                 },
                 {1, 2, 3, 6, 9, 8, 7, 4, 5}},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {5, 6, 7, 8},
                     {9, 10, 11, 12},
                 },
                 {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7}},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {5, 6, 7, 8},
                     {9, 10, 11, 12},
                     {13, 14, 15, 16},
                 },
                 {1, 2, 3, 4, 8, 12, 16, 15, 14, 13, 9, 5, 6, 7, 11, 10}},
         }) {
        const auto v1 = matrix_spiral_order(matrix);
        const auto v2 = matrix_spiral_order_v2(matrix);
        const auto v3 = matrix_spiral_order_v3(matrix);
        std::cout << __FUNCTION__ << ": matrix=" << util::dump_matrix(matrix, true)
                  << ", matrix_spiral_order=" << util::dump_array(v1) << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << ", matrix_spiral_order_v2=" << util::dump_array(v2) << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED")
                  << ", matrix_spiral_order_v3=" << util::dump_array(v3) << ", " << (exp_v == v3 ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

void run_matrix_traverse()
{
    run_matrix_diagonal_order();
    run_matrix_diagonal_rotation_order();
    run_matrix_spiral_order();
}

////////////////////////////////////////////////////////////////////////////////
//
// Find the number of islannds. Use depth search alike algorithm. Looks easy but failed to figure it out!!
// https://leetcode.com/problems/number-of-islands/description/
//
// '1' means land, '0' means water
//
// 11110
// 11010
// 11000
// 00000
// --> 1 island
//
// 11000
// 11000
// 00100
// 00011
// --> 3 islands
//
// https://www.youtube.com/watch?v=XSmgFKe-XYU&t=11s&ab_channel=HuaHua
// 1) traverse matrix row by column;
// 2) increment by 1 if matrix[r][c] == 1; then
// 3) do dfs and mark adjacent (4 directions) '1' with '0'
//
int num_islands(std::vector<std::vector<bool>> matrix)
{
    int          islands = 0;
    const size_t rows    = matrix.size();
    const size_t cols    = matrix.front().size();

    using lambda_t = std::function<void(size_t, size_t, size_t&)>;
    lambda_t dfs   = [&](size_t r, size_t c, size_t& marked) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || !matrix[r][c]) {
            return;
        }

        ++marked;
        matrix[r][c] = false;
        dfs(r, c + 1, marked);
        dfs(r + 1, c, marked);
        dfs(r, c - 1, marked);
        dfs(r - 1, c, marked);
    };

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            size_t marked = 0;
            dfs(r, c, marked);
            if (marked > 0) {
                ++islands;
                std::cout << "\t[" << r << ", " << c << "], marked=" << marked << std::endl;
            }
        }
    }
    return islands;
}

// https://www.youtube.com/watch?v=X8k48xek8g8&ab_channel=NeetCodeIO
// Alternative way:
// 1) traverse matrix row by column;
// 2) increment by 1 if matrix[r][c] == 1; then
// 3) do dfs and make element as visited
//
int num_islands_v2(const std::vector<std::vector<bool>>& matrix)
{
    const size_t rows = matrix.size();
    const size_t cols = matrix.front().size();

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    using lambda_t = std::function<void(size_t, size_t)>;
    lambda_t dfs   = [&](size_t r, size_t c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || !matrix[r][c] || visited[r][c]) {
            return;
        }

        visited[r][c] = true;
        dfs(r, c + 1);
        dfs(r + 1, c);
        dfs(r, c - 1);
        dfs(r - 1, c);
    };

    int islands = 0;
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            if (!visited[r][c] && matrix[r][c]) {
                ++islands;
                dfs(r, c);
            }
        }
    }
    return islands;
}

void run_num_islands()
{
    for (auto& matrix : {
             std::vector<std::vector<bool>>{
                 {0, 0},
                 {0, 0},
             },
             std::vector<std::vector<bool>>{
                 {0, 0},
                 {1, 0},
             },
             std::vector<std::vector<bool>>{
                 {1, 1},
                 {1, 1},
             },
             std::vector<std::vector<bool>>{
                 {1, 0},
                 {0, 1},
             },
             std::vector<std::vector<bool>>{
                 {1, 1, 1, 1, 0},
                 {1, 1, 0, 1, 0},
                 {1, 1, 0, 0, 0},
                 {0, 0, 0, 0, 0},
             },
             std::vector<std::vector<bool>>{
                 {1, 0, 0},
                 {0, 1, 1},
             },
             std::vector<std::vector<bool>>{
                 {1, 1, 0, 0, 0},
                 {1, 1, 0, 0, 0},
                 {0, 0, 1, 0, 0},
                 {0, 0, 0, 1, 1},
             },
         }) {
        std::cout << "--------------------" << std::endl;
        std::cout << util::dump_matrix(matrix) << "\n" << num_islands_v2(matrix) << num_islands(matrix) << std::endl;
    }
}

//
// A simple version with good solution:
// https://www.geeksforgeeks.org/minimum-time-taken-by-each-job-to-be-completed-given-by-a-directed-acyclic-graph/
// https://www.geeksforgeeks.org/find-the-ordering-of-tasks-from-given-dependencies/
//
// This simple version simplifies each job time, also uses the job index rather than job name.
// I don't find a way to apply its queue solution to following problem which has job time different from each other.
//
// https://techsauce.medium.com/solving-course-schedule-coding-interview-question-845a69f63b56
//
void graph_job_scheduler(const std::map<std::string, std::pair<int, std::vector<std::string>>>& libraries)
{
    // library name <> pair<time cost, dependency list>
    // std::map<std::string, std::pair<int, std::vector<std::string>>>
    struct library_t {
        bool                    started        = false;
        int                     time_remaining = 0;
        std::string             name;
        std::vector<library_t*> dependencies;
    };

    std::map<std::string, library_t> lib_map;
    for (auto& [name, content] : libraries) {
        auto& [time_cost, dependencies] = content;

        auto& lib          = lib_map[name];
        lib.name           = name;
        lib.time_remaining = time_cost;
        for (auto& dep : dependencies) {
            auto dep_lib = &lib_map[dep];
            lib.dependencies.emplace_back(dep_lib);
        }
    }

    auto print_libs = [&]() {
        for (auto& [_, lib] : lib_map) {
            std::ostringstream oss;
            oss << "\t " << lib.name << ": " << lib.started << ", " << lib.time_remaining;
            for (auto dep : lib.dependencies) {
                oss << ", [" << dep->name << ": " << dep->started << ", " << dep->time_remaining << "]";
            }
            std::cout << oss.str() << std::endl;
        }
    };

    print_libs();

    struct result_t {
        int                     time_cost = 0;
        std::vector<library_t*> running_libs;
    };
    std::vector<result_t> results;

    int step_time_cost = 0;
    while (true) {
        std::cout << "\t step_time_cost=" << step_time_cost << std::endl;

        // this happens once at the beginning, just find the jobs without any dependencies
        if (step_time_cost == 0) {
            step_time_cost = std::numeric_limits<int>::max();
            for (auto& [_, lib] : lib_map) {
                if (lib.dependencies.size() > 0)
                    continue;

                lib.started    = true;
                step_time_cost = std::min(step_time_cost, lib.time_remaining);
            }
        } else {
            result_t result;
            result.time_cost = step_time_cost;

            int tmp_step_time_cost = std::numeric_limits<int>::max();
            int running_libs       = 0;

            // Simply loop the libs and update time_remaining if it's runninng, track future step_time_cost
            // TODO: optimization, i.e. don't have to loop whole map if job is done
            for (auto& [_, lib] : lib_map) {
                if (lib.started && lib.time_remaining > 0) {
                    lib.time_remaining -= step_time_cost;

                    result.running_libs.emplace_back(&lib);

                    if (lib.time_remaining > 0) {
                        tmp_step_time_cost = std::min(tmp_step_time_cost, lib.time_remaining);
                        ++running_libs;
                    }
                }
            }

            assert(result.running_libs.size() > 0);

            step_time_cost = tmp_step_time_cost;

            // Simply loop libs and kick off jobs that have all dependencies done, just mark the 'started'
            // TODO: optimization, i.e. don't have to loop whole map if job is done
            for (auto& [_, lib] : lib_map) {
                if (lib.started)
                    continue;

                assert(lib.time_remaining > 0);

                auto iter_unfinished_dep = std::find_if(lib.dependencies.begin(), lib.dependencies.end(), [](auto v) {
                    return v->time_remaining > 0;
                });
                if (iter_unfinished_dep == lib.dependencies.end()) {
                    lib.started = true;

                    step_time_cost = std::min(step_time_cost, lib.time_remaining);
                    ++running_libs;
                }
            }

            results.emplace_back(result);

            if (running_libs == 0)
                break;
        }
        print_libs();
    }

    auto print_arr = [](auto& arr) {
        for (auto& v : arr) {
            std::cout << v->name << " ";
        }
    };
    for (auto& result : results) {
        std::cout << " time_cost=" << result.time_cost << ", running_libs=";
        print_arr(result.running_libs);
        std::cout << std::endl;
    }
};

void run_job_scheduler()
{
    using LibPair = std::pair<int, std::vector<std::string>>;
    // clang-format off
    std::map<std::string, LibPair> input = {
        {"A", LibPair(1, {})},
        {"B", LibPair(1, {"A"})},
        {"C", LibPair(3, {"A"})},
        {"D", LibPair(3, {"B"})},
        {"E", LibPair(2, {"C", "D"})},
    };
    // clang-format on

    graph_job_scheduler(input);
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/possible-bipartition/description/
// https://leetcode.com/discuss/interview-question/907250/google-phone-max-number-of-friends
//
// Bipartition: given people and dislike list, return turn if the people can be split into 2 groups, otherwise false
//
bool bipartition(const std::vector<std::array<int, 2>>& dislikes)
{
    // Just in order to reuse same test cases as max_invitation_friends
    int min_people_idx = std::numeric_limits<int>::max();
    int max_people_idx = std::numeric_limits<int>::min();
    for (auto& [p1, p2] : dislikes) {
        min_people_idx = std::min({min_people_idx, p1, p2});
        max_people_idx = std::max({max_people_idx, p1, p2});
    }

    int N = max_people_idx - min_people_idx + 1;

    // 0: init value, not dislike
    // 1: dislike
    std::vector<std::vector<int>> matrix(N, std::vector<int>(N, 0));
    for (auto& [p1, p2] : dislikes) {
        auto p1_idx            = p1 - min_people_idx;
        auto p2_idx            = p2 - min_people_idx;
        matrix[p1_idx][p2_idx] = 1;
        matrix[p2_idx][p1_idx] = 1;
    }

    // 0: init value, ungrouped
    // 1: group 1
    // 2: group 2
    std::vector<int> marked_group(N, 0);

    using lambda_t = std::function<bool(int, int, int)>;
    lambda_t dfs   = [&](size_t people_idx, int people_group, int dislike_group) {
        // mark current people with group
        marked_group[people_idx] = people_group;

        // loop current people's dislike list
        // 1) if dislike people is assigned to same group, false
        // 2) if dislike people is assigned to other group, fine
        // 3) if dislike people is not yet assigned, try assign to other group and dfs himself
        const auto& dislikes = matrix[people_idx];
        for (size_t c = 0; c < dislikes.size(); ++c) {
            if (dislikes[c]) {
                if (marked_group[c] == people_group) {
                    return false;
                } else if (marked_group[c] == 0) {
                    if (!dfs(c, dislike_group, people_group)) {
                        return false;
                    }
                } else {
                    assert(marked_group[c] == dislike_group);
                }
            }
        }
        return true;
    };

    for (int people_idx = 0; people_idx < N; ++people_idx) {
        if (marked_group[people_idx] != 0) {
            continue;
        }

        // NOTICE: it doesn't matter whether group 1 or 2 is as default, since this is depth-first-search: after this
        // run, all *associated* people on the branch are assigned to a group and none of them can (not needed since
        // the validity has been verified on the first run) change group.
        if (!dfs(people_idx, 1, 2)) {
            return false;
        }
    }

    /* bfs, not yet tested
    std::queue<int> q;
    std::vector<int> marked_group(N, 0);
    for (int people_idx = 0; people_idx < N; ++people_idx) {
        if (marked_group[people_idx] == 0) {
            marked_group[people_idx] = 1; // default group 1

            q.push(people_idx);
            while (q.empty()) {
                auto cur_idx = q.front();
                q.pop();

                const auto this_group = marked_group[cur_idx];
                assert(this_group != 0);

                const auto dislike_group = (this_group == 1 ? 2 : 1);

                for (size_t dislike_idx = 0; dislike_idx < matrix[cur_idx].size(); ++dislike_idx) {
                    if (matrix[cur_idx][dislike_idx]) {
                        if (marked_group[dislike_idx] == this_group) {
                            return false;
                        } else if (marked_group[dislike_idx] == 0) {
                            marked_group[dislike_idx] = dislike_group;
                            q.push(dislike_idx);
                        }
                    }
                }
            }
        }
    }
    */

    return true;
}
void run_bipartition()
{
    using use_case_t = std::pair<std::vector<std::array<int, 2>>, bool>;
    for (auto [dislikes, exp_v] : {
             use_case_t{
                 {
                     {1, 2},
                     {1, 3},
                     {2, 4},
                 },
                 true},
             use_case_t{
                 {
                     {1, 2},
                     {1, 3},
                     {2, 3},
                 },
                 false},
             use_case_t{
                 {
                     {1, 2},
                     {2, 3},
                     {3, 4},
                     {4, 5},
                     {1, 5},
                 },
                 false},
             use_case_t{
                 {
                     {1, 2},
                     {2, 3},
                     {3, 4},
                 },
                 true},
             use_case_t{
                 {
                     {1, 2},
                     {1, 3},
                     {2, 4},
                 },
                 true},
             use_case_t{
                 {
                     {1, 2},
                     {1, 3},
                     {2, 4},
                     {2, 5},
                     {3, 5},
                     {6, 3},
                 },
                 true},
         }) {
        const auto v = bipartition(dislikes);
        std::cout << __FUNCTION__ << ": dislikes=" << util::dump_matrix(dislikes) << ", bipartition=" << std::boolalpha
                  << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Similar questions but not sure if same answer:
// https://github.com/doocs/leetcode/blob/main/solution/1800-1899/1820.Maximum%20Number%20of%20Accepted%20Invitations/README_EN.md
//
// You want to invite as many friends to your party as possible. You're given a list of tuples where each tuple consists
// of two friend ids and they don't like each other. Your task is to invite as many friends as possible. Output of the
// task - max number of friends you can invite which don't have conflicts with each other.
//
// Input: [(1, 2), (2, 3), (3, 4)]
// Ouput: 2 (it's either (1, 3) or (2, 4) or (1, 4)
//
int max_invitation_friends(const std::vector<std::array<int, 2>>& dislikes)
{
    std::multimap<int, int> dislikes_map;
    for (auto& [p1, p2] : dislikes) {
        dislikes_map.emplace(p1, p2);
        dislikes_map.emplace(p2, p1);
    }

    using lambda_t = std::function<int(const std::multimap<int, int>&)>;
    lambda_t impl  = [&impl](const std::multimap<int, int>& local_dislikes_map) {
        if (local_dislikes_map.size() == 0)
            return 0;

        auto print_map = [&]() {
            int prev_k = 0;
            for (auto [k, v] : local_dislikes_map) {
                if (prev_k != k) {
                    if (prev_k != 0) {
                        std::cout << "]";
                    }
                    std::cout << "\n\t[" << k << ": " << v;
                    prev_k = k;
                } else {
                    std::cout << ", " << v;
                }
            }
            if (local_dislikes_map.size() > 0) {
                std::cout << "]" << std::endl;
            }
        };

        std::cout << "---------------";
        print_map();

        auto remove_not_invited_node = [](auto& local_map, int b, int exclude_key = std::numeric_limits<int>::min()) {
            auto [iter_b, _] = local_map.equal_range(b);
            while (iter_b != local_map.end() && iter_b->first == b) {
                auto d                          = iter_b->second;
                auto [iter_d_begin, iter_d_end] = local_map.equal_range(d);
                for (auto iter_d = iter_d_begin; iter_d != iter_d_end; ++iter_d) {
                    // NOTICE: exclude_key is the key that is invited, don't touch it since the associated entry is
                    // erased outside in order to gain the following iterator
                    if (iter_d->second == b && iter_d->first != exclude_key) {
                        // NOTICE: if key has only one entry, don't erase it, since this key must be valid to be
                        // invited
                        if (std::distance(iter_d_begin, iter_d_end) != 1) {
                            local_map.erase(iter_d);
                        }
                        break;
                    }
                }
                iter_b = local_map.erase(iter_b);
            }
        };

        int count_with_p    = 0;
        int count_without_p = 0;
        auto [p, _]         = *local_dislikes_map.begin();
        // P > B > D, E
        //   > C > E, F

        // p is invited
        {
            auto local_map   = local_dislikes_map;
            auto [iter_p, _] = local_map.equal_range(p);
            while (iter_p != local_map.end() && iter_p->first == p) {
                auto b = iter_p->second;
                remove_not_invited_node(local_map, b, p);

                iter_p = local_map.erase(iter_p);
            }

            count_with_p += 1 + impl(local_map);
            std::cout << "\t count_with_p=" << count_with_p << std::endl;
        }

        // p is not invited
        {
            auto local_map = local_dislikes_map;
            remove_not_invited_node(local_map, p);

            count_without_p = impl(local_map);
            std::cout << "\t count_without_p=" << count_without_p << std::endl;
        }

        return std::max(count_with_p, count_without_p);
    };

    return impl(dislikes_map);
}
// TODO: don't work
int max_invitation_friends_v2(const std::vector<std::array<int, 2>>& dislikes)
{
    struct people_t {
        bool                   visited = false;
        int                    id      = 0;
        std::vector<people_t*> dislikes;
    };

    std::map<int, people_t> all_people;

    for (auto [p1, p2] : dislikes) {
        auto& people_1 = all_people[p1];
        people_1.id    = p1;
        auto& people_2 = all_people[p2];
        people_2.id    = p2;
        people_1.dislikes.push_back(&people_2);
        people_2.dislikes.push_back(&people_1);
    }

    std::vector<people_t*> people_marked;

    // TODO: don't work
    using lambda_t = std::function<int()>;
    lambda_t impl  = [&]() {
        auto print_map = [&]() {
            for (auto [_, p] : all_people) {
                std::cout << "\n\t[" << p.id << "(" << std::boolalpha << p.visited << ")";
                for (size_t i = 0; i < p.dislikes.size(); ++i) {
                    if (i == 0) {
                        std::cout << ": ";
                    }
                    std::cout << p.dislikes[i]->id;
                    if (i != p.dislikes.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]";
            }
            std::cout << std::endl;
        };

        std::cout << "---------------";
        print_map();

        int count_with_p    = 0;
        int count_without_p = 0;
        for (auto& [_, people] : all_people) {
            if (people.visited)
                continue;

            people.visited = true;
            people_marked.push_back(&people);

            // people is not invited
            count_without_p = impl();
            std::cout << "p=" << people.id << ", count_without_p=" << count_without_p
                      << ", marked=" << people_marked.size() << std::endl;
            for (auto marked : people_marked) {
                marked->visited = false;
            }
            people_marked.clear();

            people.visited = true;
            people_marked.push_back(&people);

            // people is invited
            for (auto dislike : people.dislikes) {
                // assert(!dislike->visited);
                if (!dislike->visited) {
                    dislike->visited = true;
                    people_marked.push_back(dislike);
                }
            }
            count_with_p = 1 + impl();
            std::cout << "p=" << people.id << ", count_with_p=" << count_with_p << ", marked=" << people_marked.size()
                      << std::endl;

            for (auto marked : people_marked) {
                marked->visited = false;
            }
            people_marked.clear();
        }
        return std::max(count_with_p, count_without_p);
    };

    return impl();
}

/* TODO: DFS, go through all people, for each person, choose to invite or not invite, find the max number
int max_invitation_friends_v3(const std::vector<std::array<int, 2>>& dislikes)
{
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, std::unordered_set<int>> dislikes_map;
    for (auto [a, b] : dislikes) {
        dislikes_map[a].emplace(b);
        dislikes_map[b].emplace(a);
        visited.emplace(a);
        visited.emplace(b);
    }

    auto impl = [&visited, &dislikes_map](int i) {
        if (visited[i]) {
            return 0;
        }

        visited[i] = true;

        int result = 0;
        std::unordered_set<int> invited_set;
        std::unordered_set<int> not_invited_set;

        // not invite i
        not_invited_set.emplace(i);
        for (auto j : dislikes_map[i]) {
            // invite or not invite j
            result = std::max(result, impl(j));
        }

        // invite i
        not_invited_set.erase(i);
        invited_set.emplace(i);
        result = std::max(result, impl(i));

        max_invitation_map[i] = std::max(max_invitation_map[i], result);
    };

    std::unordered_map<int, int> max_invitation_map;
    int result = 0;
    for (auto [i, _] : visited) {
        // invite or not invite i
        impl(i);
    }
    return result;
}
*/

void run_max_invitition_friends()
{
    for (auto& input : {
             std::vector<std::array<int, 2>>{
                 {1, 2},
                 {1, 3},
                 {2, 4},
             },
             std::vector<std::array<int, 2>>{
                 {1, 2},
                 {1, 3},
                 {2, 3},
             },
             std::vector<std::array<int, 2>>{
                 {1, 2},
                 {2, 3},
                 {3, 4},
                 {4, 5},
                 {1, 5},
             },
             std::vector<std::array<int, 2>>{
                 {1, 2},
                 {2, 3},
                 {3, 4},
             },
             std::vector<std::array<int, 2>>{
                 {1, 2},
                 {1, 3},
                 {2, 4},
             },
             std::vector<std::array<int, 2>>{
                 {1, 2},
                 {1, 3},
                 {2, 4},
                 {2, 5},
                 {3, 5},
                 {6, 3},
             },
         }) {
        std::cout << max_invitation_friends(input) << std::endl;
        // std::cout << max_invitation_friends_v2(input) << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// There are 'm' boys and 'n' girls in a class attending an upcoming party.
//
// You are given an 'm x n' integer matrix grid, where grid[i][j] equals 0 or 1. If grid[i][j] == 1, then that means the
// i-th boy can invite the jth girl to the party. A boy can invite at most one girl, and a girl can accept at most one
// invitation from a boy.
//
// Return the maximum possible number of accepted invitations.
//
// leetcode 1820
//
int max_invitation_boy_girl(const std::vector<std::vector<bool>>& possibilities)
{
    auto print_matrix = [&]() {
        for (size_t r = 0; r < possibilities.size(); ++r) {
            if (r == 0) {
                std::cout << "\n[";
            } else {
                std::cout << " ";
            }
            for (size_t c = 0; c < possibilities[r].size(); ++c) {
                std::cout << possibilities[r][c];
                if (c != possibilities[r].size() - 1) {
                    std::cout << " ";
                }
            }
            if (r == possibilities.size() - 1) {
                std::cout << "]";
            }
            std::cout << "\n";
        }
    };

    std::cout << "---------------";
    print_matrix();

    const size_t     boys  = possibilities.size();
    const size_t     girls = possibilities.front().size();
    std::vector<int> invitations(girls, -1);

    using lambda_t = std::function<bool(size_t, std::vector<bool>&)>;
    lambda_t impl  = [&](size_t boy, std::vector<bool>& visited_girls) {
        // iterate all possible girls for current boy
        for (size_t girl = 0; girl < possibilities[boy].size(); ++girl) {
            // 1) do nothing if boy cannot invite this girl; or
            // 2) the girl has been 'visited' already, this is to avoid dead-loop: handle same girl for multi times
            if (!possibilities[boy][girl] || visited_girls[girl])
                continue;

            visited_girls[girl] = true;

            // 1) if the girl has not yet been invited;
            // 2) if the girl has been invited but that boy can switch to another girl
            if (invitations[girl] < 0 || impl(invitations[girl], visited_girls)) {
                invitations[girl] = boy;
                return true;
            }
        }
        return false;
    };

    int max_invitations = 0;
    for (size_t boy = 0; boy < boys; ++boy) {
        // this state is per boy/cycle in order to avoid duplicate handling on same girl
        std::vector<bool> visited(girls, false);
        if (impl(boy, visited)) {
            ++max_invitations;
        }
    }
    return max_invitations;
}

void run_max_invitition_boy_girl()
{
    using use_case_t = std::pair<std::vector<std::vector<bool>>, int>;
    for (auto& [possibilities, exp_v] : {
             use_case_t{
                 {
                     {1, 1, 1},
                     {1, 0, 1},
                     {0, 0, 1},
                 },
                 3},
             use_case_t{
                 {
                     {1, 0, 1, 0},
                     {1, 0, 0, 0},
                     {0, 0, 1, 0},
                     {1, 1, 1, 0},
                 },
                 3},
         }) {
        const auto v = max_invitation_boy_girl(possibilities);
        std::cout << __FUNCTION__ << ": possibilities=" << util::dump_matrix(possibilities)
                  << ", max_invitation_boy_girl=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// https://www.geeksforgeeks.org/minimum-steps-reach-target-knight/
// Upgrade version: https://imgur.com/a/lJWoY6p
//
// TODO: this can also be implemented by DFS, each visited nodes hold min moves until destination node
//
// This solution applies to any graph: given a directional graph, find the min/max moves from one vetex to the other
//
// But if edge/vertex has weight, this won't work becausee the 'visited' and value check logic won't apply!!
//
int min_knight_move(int max_rows, int max_cols, const std::array<int, 2>& from, const std::array<int, 2>& to)
{
    using pos_t          = std::array<int, 2>;
    auto is_in_the_range = [&](const pos_t& pos) {
        const auto row = pos[0];
        const auto col = pos[1];
        return row >= 0 && col >= 0 && row < max_rows && col < max_cols;
    };

    struct state_t {
        int   moves = 0;
        pos_t pos{0, 0};
    };

    // Or just contains (pos_x, pos_y, moves) as std::queue<std::tuple<int, int, int>> q;
    std::queue<state_t> q;
    q.push({0, from});

    int                            min_moves = std::numeric_limits<int>::max();
    std::vector<std::vector<bool>> visited(max_rows, std::vector<bool>(max_cols, false));

    // BFS, inside loop all possibilites (children), push to queue and iterate
    while (!q.empty()) {
        auto state = std::move(q.front());
        q.pop();

        // std::cout << "pop state: [" << state.pos[0] << ", " << state.pos[1] << "], moves=" << state.moves <<
        // std::endl;

        if (state.pos == to) {
            // std::cout << "reached target: min_moves=" << min_moves << " state.moves=" << state.moves << std::endl;
            //  TODO: likely this min value check is not necessary, how is it ensured???
            //  just 'break' than 'continue' then;
            //
            //  Reasoning: we don't need min value check here because the solution is BFS, the first route reaches this
            //  point must be the optimal one! Just 'break' and return;
            min_moves = std::min(state.moves, min_moves);
            continue;
        }
        for (const auto [offset_x, offset_y] : {
                 pos_t{-2, -1},
                 pos_t{-1, -2},
                 pos_t{1, -2},
                 pos_t{2, -1},
                 pos_t{-2, 1},
                 pos_t{-1, 2},
                 pos_t{1, 2},
                 pos_t{2, 1},
             }) {
            auto next_pos = state.pos;
            next_pos[0] += offset_x;
            next_pos[1] += offset_y;
            if (!is_in_the_range(next_pos)) {
                continue;
            }

            // TODO: should I compare the #moves even if the next_pos has been visited?
            // How do we know the visited route is optimized???
            //
            // Reasoning: this is same reasoning as above min value check, no #moves check is needed here.
            if (visited[next_pos[0]][next_pos[1]]) {
                continue;
            }
            visited[next_pos[0]][next_pos[1]] = true;

            q.push({state.moves + 1, next_pos});
            // std::cout << "push state: [" << q.back().pos[0] << ", " << q.back().pos[1] << "], moves=" <<
            // q.back().moves << std::endl;
        }
    }
    return min_moves;
}

void run_min_knight_move()
{
    // <rows, cols, knight, target, expected result>
    using use_case_t = std::tuple<int, int, std::array<int, 2>, std::array<int, 2>, int>;
    for (auto [rows, cols, knight, target, exp_v] : {
             use_case_t{6, 6, {1, 3}, {5, 0}, 3},
             use_case_t{8, 8, {4, 2}, {2, 6}, 2},
             use_case_t{30, 30, {0, 0}, {29, 29}, 20},
         }) {
        const auto v = min_knight_move(rows, cols, knight, target);
        std::cout << __FUNCTION__ << ": rows=" << rows << ", cols=" << cols << ", knight=(" << knight[0] << ", "
                  << knight[1] << "), target=(" << target[0] << ", " << target[1] << "); min_knight_move=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 64
//
// Given 'm x n' matrix with all positive integers, find the min sum path from left-top to right-bottom.
// You can only move either right or down at each point.
//
// Both DFS and BFS should work. This is DFS implementation.
//
int min_path_sum_matrix(const std::vector<std::vector<int>>& matrix)
{
    const int rows = matrix.size();
    const int cols = matrix.front().size();

    std::vector<std::vector<int>> results(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));

    // DFS
    using lambda_t = std::function<void(int, int, int)>;
    lambda_t dfs   = [&](int x, int y, int sum_so_far) {
        if (x >= rows || y >= cols) {
            return;
        }

        auto new_sum = sum_so_far + matrix[x][y];
        // NOTICE: results[x][y] may contain value, meaning some node is allowed to be visited for multi times in
        // different routes, and we try all the routes and pick the optimal one
        if (new_sum > results[x][y]) {
            // do nothing, do not proceed on the route
            // std::cout << "visited before and new path is worse, ignore" << std::endl;
            return;
        }
        results[x][y] = new_sum;

        if (x == rows - 1 && y == cols - 1) {
            return;
        }
        dfs(x + 1, y, new_sum);
        dfs(x, y + 1, new_sum);
    };

    dfs(0, 0, 0);

    return results.back().back();
}

//
// This gives even better/cleaner solution by using dynamic programming. NICE!
//
int min_path_sum_matrix_v2(const std::vector<std::vector<int>>& matrix)
{
    const int rows = matrix.size();
    const int cols = matrix.front().size();

    std::vector<std::vector<int>> results(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));

    results[0][0] = matrix[0][0];
    for (int r = 1; r < rows; ++r) {
        results[r][0] = results[r - 1][0] + matrix[r][0];
    }

    for (int c = 1; c < cols; ++c) {
        results[0][c] = results[0][c - 1] + matrix[0][c];
    }

    for (int r = 1; r < rows; ++r) {
        for (int c = 1; c < cols; ++c) {
            results[r][c] = matrix[r][c] + std::min(results[r - 1][c], results[r][c - 1]);
        }
    }
    return results.back().back();
}

// https://leetcode.com/discuss/interview-question/1292854/faang-get-the-minimum-value-of-maximum-values-of-2d-matrix-path
//
// This is a variant, for all the max cell values on each path, find the min value
//
// {1, 3, 1},
// {1, 5, 1},
// {4, 2, 1},
//
// path 1: 1 > 3 > 1 > 1 > 1, max value 3
// path 2: 1 > 1 > 4 > 2 > 1, max value 4
// path 3: 1 > 1 > 5 > 1 > 1, max value 5
// ...
// So the answer is 3!
//
int min_value_max_on_each_path(const std::vector<std::vector<int>>& matrix)
{
    const int rows = matrix.size();
    const int cols = matrix.front().size();

    std::vector<std::vector<int>> results(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));

    results[0][0] = matrix[0][0];
    for (int r = 1; r < rows; ++r) {
        results[r][0] = std::max(results[r - 1][0], matrix[r][0]);
    }

    for (int c = 1; c < cols; ++c) {
        results[0][c] = std::max(results[0][c - 1], matrix[0][c]);
    }

    for (int r = 1; r < rows; ++r) {
        for (int c = 1; c < cols; ++c) {
            results[r][c] = std::max(matrix[r][c], std::min(results[r - 1][c], results[r][c - 1]));
        }
    }
    return results.back().back();
}

void run_min_path_sum_matrix()
{
    using use_case_t = std::pair<std::vector<std::vector<int>>, int>;
    for (auto [matrix, exp_v] : {
             use_case_t{
                 {
                     {1, 3, 1},
                     {1, 5, 1},
                     {4, 2, 1},
                 },
                 7},
             use_case_t{
                 {
                     {1, 2, 3},
                     {4, 5, 6},
                 },
                 12},
             use_case_t{
                 {
                     {1, 3, 1},
                     {1, 2, 1},
                     {4, 2, 1},
                 },
                 6},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {12, 14, 15, 16},
                     {10, 22, 32, 34},
                     {9, 8, 7, 5},
                 },
                 52},
         }) {
        std::cout << "--------------------" << std::endl;
        std::cout << util::dump_matrix(matrix) << std::endl;

        const auto v1 = min_path_sum_matrix(matrix);
        const auto v2 = min_path_sum_matrix_v2(matrix);

        std::cout << __FUNCTION__ << ": min_path_sum_matrix=" << v1 << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << ", min_path_sum_matrix_v2=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

void run_min_value_max_on_each_path()
{
    using use_case_t = std::pair<std::vector<std::vector<int>>, int>;
    for (auto [matrix, exp_v] : {
             use_case_t{
                 {
                     {1, 3, 1},
                     {1, 5, 1},
                     {4, 2, 1},
                 },
                 3},
             use_case_t{
                 {
                     {1, 2, 3},
                     {4, 5, 6},
                 },
                 6},
             use_case_t{
                 {
                     {1, 3, 1},
                     {1, 2, 1},
                     {4, 2, 1},
                 },
                 2},
             use_case_t{
                 {
                     {1, 2, 3, 4},
                     {12, 14, 15, 16},
                     {10, 22, 32, 34},
                     {9, 8, 7, 5},
                 },
                 12},
         }) {
        std::cout << "--------------------" << std::endl;
        std::cout << util::dump_matrix(matrix) << std::endl;

        const auto v = min_value_max_on_each_path(matrix);
        std::cout << __FUNCTION__ << ": min_value_max_on_each_path=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Graph question, given edges, you cannot pick up the neighbors. Tell the max number of nodes you can choose.
//
// This is same question as friend invitation: given dislike friends, tell the max number of frieds you can invite.
//
int graph_max_nodes_exclu_neighbors(const std::vector<std::array<int, 2>>& edges)
{
    struct node_t {
        int                  value = 0;
        std::vector<node_t*> neighbors;
    };

    std::unordered_map<int, node_t> graph;
    for (size_t i = 0; i < edges.size(); ++i) {
        const auto [a, b] = edges[i];
        graph[a].neighbors.emplace_back(&graph[b]);
        graph[b].neighbors.emplace_back(&graph[a]);

        graph[a].value = a;
        graph[b].value = b;
    }

    // Need this map for visited nodes since same node is next to multiple neighbors
    std::unordered_map<node_t*, int> visited;

    // DFS
    std::function<int()> impl = [&]() {
        node_t* current = nullptr;
        for (auto& [_, node] : graph) {
            if (visited.find(&node) == visited.end()) {
                current = &node;
                break;
            }
        }
        if (!current)
            return 0;

        std::cout << "--current=" << current->value << ", visited=" << visited.size() << std::endl;

        visited[current]++;

        //
        // doesn't count the node
        const auto count_without_node = impl();

        //
        // count the node

        // 'neighbor' may be added before, increase its freq
        for (auto neighbor : current->neighbors) {
            visited[neighbor]++;
        }

        const auto count_with_node = 1 + impl();

        // restore the state
        for (auto neighbor : current->neighbors) {
            // 'neighbor' may be added before, don't erase unless the freq == 0
            if (--visited[neighbor] <= 0) {
                visited.erase(neighbor);
            }
        }

        // there is no way 'current' node is added before this cycle
        assert(visited[current] == 1);
        if (--visited[current] == 0) {
            visited.erase(current);
        }

        return std::max(count_with_node, count_without_node);
    };

    return impl();
}

void run_max_nodes_exclu_neighbors()
{
    using use_case_t = std::tuple<std::vector<std::array<int, 2>>, int>;
    for (auto [edges, exp_v] :
         {use_case_t{
              {
                  {1, 2},
                  {1, 3},
                  {2, 4},
              },
              2},
          use_case_t{
              {
                  {1, 2},
                  {1, 3},
                  {2, 3},
              },
              1},
          use_case_t{
              {
                  {1, 2},
                  {2, 3},
                  {3, 4},
                  {4, 5},
                  {1, 5},
              },
              2},
          use_case_t{
              {
                  {1, 2},
                  {2, 3},
                  {3, 4},
              },
              2},
          use_case_t{
              {
                  {1, 2},
                  {1, 3},
                  {2, 4},
                  {2, 5},
                  {3, 5},
                  {6, 3},
              },
              4}}) {
        const auto v = graph_max_nodes_exclu_neighbors(edges);
        std::cout << __FUNCTION__ << ": graph_max_nodes_exclu_neighbors=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Given a directed acyclic graph, find the set of dominant vertices for each vertex of the graph.
//
// Assume the vertices consecutive integers [0, vertices_count).
//
// https://www.geeksforgeeks.org/find-the-dominators-for-every-vertex-in-a-given-dag-directed-acyclic-graph/
// This page describes this problem very well. Implement by DFS?
//
std::vector<std::set<int>>
graph_find_dominator(int vertices_count, const std::vector<std::pair<int, int>>& directed_edges)
{
    std::vector<std::vector<int>> graph(vertices_count, std::vector<int>{});
    std::vector<int>              parent_count(vertices_count, 0);
    for (auto [parent, child] : directed_edges) {
        // for each parent vertex, track all its children
        graph[parent].emplace_back(child);
        // for each child vertex, count the number of parent vertices, so called 'inDegree'
        parent_count[child]++;
    }

    std::vector<std::set<int>> dominators(vertices_count, std::set<int>{});
    std::queue<int>            q;

    // find all verties that have no parent
    for (size_t i = 0; i < parent_count.size(); ++i) {
        if (parent_count[i] == 0) {
            q.push(i);
            dominators[i].emplace(i);
        }
    }

    // BFS: can this be done by DFS??
    while (!q.empty()) {
        const auto v = q.front();
        q.pop();

        for (const auto child : graph[v]) {
            if (dominators[child].empty()) {
                dominators[child] = dominators[v];
                dominators[child].emplace(child);
            } else {
                std::set<int> tmp;
                std::set_intersection(
                    dominators[child].begin(), dominators[child].end(), dominators[v].begin(), dominators[v].end(),
                    std::inserter(tmp, tmp.begin()));
                dominators[child] = std::move(tmp);
            }

            if (--parent_count[child] == 0) {
                dominators[child].emplace(child);
                q.push(child);
            }
        }
    }
    return dominators;
}

void run_graph_find_dominator()
{
    auto print = [](const auto& dominators) {
        std::cout << "---------------" << std::endl;
        for (size_t i = 0; i < dominators.size(); ++i) {
            std::cout << "vertex " << i << ": ";
            for (auto v : dominators[i]) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }
    };

    using use_case_t = std::tuple<int, std::vector<std::pair<int, int>>, std::vector<std::set<int>>>;
    for (auto [vertex_count, edges, exp_v] : {
             use_case_t{
                 5,
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 {
                     {0},          // vertex 0
                     {0, 1},       // vertex 1
                     {0, 2},       // vertex 2
                     {0, 3},       // vertex 3
                     {0, 3, 4},    // vertex 4
                 }},
             use_case_t{
                 4,
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},

                 },
                 {
                     {0},       // vertex 0
                     {0, 1},    // vertex 1
                     {2},       // vertex 2
                     {3},       // vertex 3
                 }},
         }) {
        const auto v = graph_find_dominator(vertex_count, edges);
        std::cout << __FUNCTION__ << ": graph_find_dominator=\n";
        print(v);
        std::cout << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

//
// Given a directed acyclic graph and weights of each vertex, find the max abs difference between the weights of any
// node and any of its ancestor
//
// Assume the vertices consecutive integers [0, vertices_count).
//
// https://www.geeksforgeeks.org/maximum-difference-between-node-and-its-ancestor-in-a-directed-acyclic-graph-dag/
//
int graph_find_max_difference(const std::vector<int>& weights, const std::vector<std::pair<int, int>>& directed_edges)
{
    const auto                    vertices_count = weights.size();
    std::vector<std::vector<int>> graph(vertices_count, std::vector<int>{});
    std::vector<int>              parent_count(vertices_count, 0);
    for (auto [parent, child] : directed_edges) {
        // for each parent vertex, track all its children
        graph[parent].emplace_back(child);
        // for each child vertex, count the number of parent vertices, so called 'inDegree'
        parent_count[child]++;
    }

    std::vector<int> vertices_no_parent;

    // find all verties that have no parent
    for (size_t i = 0; i < parent_count.size(); ++i) {
        if (parent_count[i] == 0) {
            vertices_no_parent.emplace_back(i);
        }
    }

    int max_diff = 0;

    // DFS: seems not easy to reach the goal by using BFS
    std::function<void(int, int, int)> dfs = [&](int idx_v, int min_weight, int max_weight) {
        for (const auto idx_child : graph[idx_v]) {
            max_diff = std::max(
                {max_diff, std::abs(weights[idx_child] - min_weight), std::abs(weights[idx_child] - max_weight)});

            int min_weight_tmp = std::min(weights[idx_child], min_weight);
            int max_weight_tmp = std::max(weights[idx_child], max_weight);

            dfs(idx_child, min_weight_tmp, max_weight_tmp);
        }
    };

    for (auto idx_v : vertices_no_parent) {
        dfs(idx_v, weights[idx_v], weights[idx_v]);
    }
    return max_diff;
}

void run_graph_find_max_difference()
{
    // <edges, weights, exp_v>
    using use_case_t = std::tuple<std::vector<std::pair<int, int>>, std::vector<int>, int>;

    for (auto [edges, weights, exp_v] : {
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 {1, 3, 5, 7, 9},
                 8},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 {9, 7, 5, 3, 1},
                 8},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 {1, 2, 3, 4, 5},
                 4},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 {3, 3, 3, 3, 3},
                 0},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 {5, 3, 9, 6, 4},
                 5},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},
                 },
                 {9, 2, 7, 8},
                 7},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},
                 },
                 {1, 2, 3, 4},
                 2},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},
                 },
                 {3, 0, 2, 8},
                 6},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},
                 },
                 {1, 1, 1, 1},
                 0},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},
                 },
                 {2, 6, 7, 3},
                 5},
         }) {
        const auto v = graph_find_max_difference(weights, edges);
        std::cout << __FUNCTION__ << ": weights=" << util::dump_array(weights, std::numeric_limits<int>::max())
                  << ", graph_find_max_difference=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

//
// Given a directed acyclic graph, find the max depth (longest path) between any node and any of its ancestor
//
// Assume the vertices consecutive integers [0, vertices_count).
//
// https://www.geeksforgeeks.org/dynamic-programming-dp-and-directed-acyclic-graphs-dag/
// https://www.geeksforgeeks.org/longest-path-in-a-directed-acyclic-graph-dynamic-programming/
// This belongs to dynamic programming!
//
std::pair<std::vector<int>, int>
graph_find_max_depth(int vertices_count, const std::vector<std::pair<int, int>>& directed_edges)
{
    std::vector<std::vector<int>> graph(vertices_count, std::vector<int>{});
    std::vector<int>              parent_count(vertices_count, 0);
    for (auto [parent, child] : directed_edges) {
        // for each parent vertex, track all its children
        graph[parent].emplace_back(child);
        // for each child vertex, count the number of parent vertices, so called 'inDegree'
        parent_count[child]++;
    }

    std::vector<int> vertices_no_parent;

    // find all verties that have no parent
    for (size_t i = 0; i < parent_count.size(); ++i) {
        if (parent_count[i] == 0) {
            vertices_no_parent.emplace_back(i);
        }
    }

    // DFS: for specific vertex 'v', its depths[v] = max(depths[v], depths[parent_of_v] + 1); iterate all 'parent_of_v'
    // TODO: BFS is also working!
    std::vector<int>         depths(vertices_count, 0);
    int                      max_depth = 0;
    std::function<void(int)> dfs       = [&](int idx_parent) {
        for (auto idx_child : graph[idx_parent]) {
            depths[idx_child] = std::max(depths[idx_child], depths[idx_parent] + 1);
            max_depth         = std::max(depths[idx_child], max_depth);

            dfs(idx_child);
        }
    };

    for (auto idx_v : vertices_no_parent) {
        dfs(idx_v);
    }

    return {depths, max_depth};
}

void run_graph_find_max_depth()
{
    // <edges, vertex_count, exp_v>
    using use_case_t = std::tuple<std::vector<std::pair<int, int>>, int, int>;

    for (auto [edges, vertex_count, exp_v] : {
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 2},
                     {2, 3},
                     {1, 3},
                 },
                 4,
                 3},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {2, 3},
                     {3, 4},
                 },
                 5,
                 3},
             use_case_t{
                 {
                     {0, 1},
                     {0, 2},
                     {3, 2},
                 },
                 4,
                 1},
             use_case_t{
                 {
                     {4, 1},
                     {4, 2},
                     {4, 0},
                     {1, 2},
                     {0, 2},
                     {0, 3},
                     {1, 3},
                     {3, 2},
                 },
                 5,
                 3},
         }) {
        const auto result = graph_find_max_depth(vertex_count, edges);
        std::cout << __FUNCTION__ << ": graph_find_max_depth=" << result.second << ", "
                  << (exp_v == result.second ? "SUCCESS" : "FAILED") << std::endl;
    }
}

//
// Find factorization of given integer in ascend order, i.e. 10 = 2 * 5, 50 = 2 * 5 * 5, 100 = 2 * 2 * 5 * 5
//
std::vector<int> find_prime_factorization(int num)
{
    std::vector<int> factorization;
    int              n = 2;
    while (n * n <= num) {
        if (num % n == 0) {
            factorization.push_back(n);
            num /= n;
        } else {
            ++n;
        }
    }
    if (num != 1 || factorization.empty()) {
        factorization.push_back(num);
    }
    return factorization;
}

// Find all divisors of given integer in ascend order, i.e. 4 > (2), 10 > (2, 5), 20 > (2, 4, 5, 10)
std::vector<int> find_all_divisors(int num)
{
    std::vector<int> divisors;
    int              n = 2;
    for (; n * n < num; ++n) {
        if (num % n == 0) {
            divisors.push_back(n);
        }
    }
    // below commented code also works but this one performs better
    int count = divisors.size();
    while (count--) {
        divisors.push_back(num % divisors[count]);
    }
    /*
    if (n * n > num) {
        --n;
    }
    for (; n > 1; --n) {
        if (num % n == 0) {
            divisors.push_back(num / n);
        }
    }
    */

    if (divisors.empty()) {
        divisors.push_back(num);
    }
    return divisors;
}
std::vector<int> find_all_divisors_v2(int num)
{
    std::vector<int> divisors;
    std::vector<int> divisors2;
    int              n = 2;
    for (; n * n <= num; ++n) {
        if (num % n == 0) {
            divisors.push_back(n);
            if (auto k = num / n; k != n) {
                divisors2.push_back(k);
            }
        }
    }
    while (!divisors2.empty()) {
        divisors.push_back(divisors2.back());
        divisors2.pop_back();
    }
    if (divisors.empty()) {
        divisors.push_back(num);
    }
    return divisors;
}

// Find all primes not greater than cap value, time complexity O(n*log(log(n)))
std::vector<int> find_all_primes(int cap)
{
    std::vector<bool> isPrime(cap + 1, true);
    for (int i = 2; i * i <= cap; ++i) {
        // TODO: can be optimized, don't need this if branch, and following loop start from some place after sqrt(cap)
        if (!isPrime[i])
            continue;
        for (int j = i * i; j <= cap; j += i) {
            isPrime[j] = false;
        }
    }
    std::vector<int> primes;
    for (int i = 2; i <= cap; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}
std::vector<int> find_all_primes_v2(int cap)
{
    std::vector<int>  primes;
    std::vector<bool> isPrime(cap + 1, true);
    for (int i = 2; i <= cap; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
            for (int j = i; j <= cap; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return primes;
}

// https://leetcode.com/problems/clumsy-factorial/
//
// clumsy(4)  = 4 * 3 / 2 + 1 = 7
// clumsy(10) = 10 * 9 / 8 + 7 - 6 * 5 / 4 + 3 - 2 * 1 = 12
//
// 1) The operations */+- are applied from left to right iteratively.
// 2) The division used is floor division such that 10 * 9 / 8 = 90 / 8 = 11.
//
// Besides of following solution, we can also use std::stack
int clumsy_factorial(int n)
{
    int result = 0;
    int i      = n;
    while (i > 0) {
        // First */ is positive while all rest are negative
        int tmp = (i == n ? i : -i);
        if (i > 1) {
            tmp *= (i - 1);
        }
        if (i > 2) {
            tmp /= (i - 2);
        }
        if (i > 3) {
            tmp += (i - 3);
        }
        result += tmp;
        i -= 4;
    }
    return result;
}

void run_factorization_divisor_prime()
{
    for (auto n : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 18, 20, 50, 80, 100}) {
        {
            auto factorization = find_prime_factorization(n);
            std::cout << n << "(factorization): " << util::dump_array(factorization) << std::endl;
        }

        {
            auto divisors = find_all_divisors(n);
            std::cout << n << "(divisors): " << util::dump_array(divisors);
            auto v2 = find_all_divisors_v2(n);
            if (v2 != divisors) {
                std::cout << "\n\terror: find_all_divisors_v2 is different: " << util::dump_array(v2);
            }
            std::cout << std::endl;
        }

        {
            auto primes = find_all_primes(n);
            std::cout << n << "(primes): " << util::dump_array(primes);
            auto v2 = find_all_primes_v2(n);
            if (v2 != primes) {
                std::cout << "\n\terror: find_all_primes_v2 is different: " << util::dump_array(v2);
            }
            std::cout << std::endl;
        }

        {
            std::cout << n << "(clumsy): " << clumsy_factorial(n) << std::endl;
            std::cout << std::endl;
        }
    }
}

// Some question from Jump
bool valid_sentence(const std::string& input)
{
    if (input.size() < 2)
        return false;

    int count_space = 0;
    for (size_t i = 0; i < input.size() - 1; ++i) {
        const auto ch = input[i];
        if (i == 0) {
            if (ch < 'A' || ch > 'Z') {
                return false;
            }
            continue;
        }

        if (ch == ' ') {
            if (++count_space > 1) {
                return false;
            }
            continue;
        }
        count_space = 0;

        // (,,;,:) or terminal marks (.,?,!,‽)
        if ((ch < 'a' || ch > 'z') && (ch != ',' && ch != ';' && ch != ':')) {
            return false;
        }
    }
    const auto ch = input.back();
    if (count_space != 0 || (ch != '.' && ch != '?' && ch != '!')) {
        return false;
    }

    return true;
}

void run_valid_sentence()
{
    for (std::string input : {
             "n",
             "a!",
             "A.",
             "Awesome!",
             "awesome?",
             "A good thing.",
             "A not that good,",
             "A not  that good!",
             "What the hell0 name",
             "Bad ending .",
             "Bad ending again$",
         }) {
        std::cout << "\"" << input << "\"" << (valid_sentence(input) ? " IS " : " IS NOT ") << "a sentence"
                  << std::endl;
    }
}

