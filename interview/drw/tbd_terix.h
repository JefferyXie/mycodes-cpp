#include <algorithm>
#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <iomanip>
#include <istream>
#include <limits>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <strstream>
#include <utility>
#include <variant>
#include <vector>
#include "header.h"
#include "../test/Test_Fixture.h"
#include "../test/Test_language.h"
#include "../test/Test_algorithm.h"
#include "../test/Test_multithreading.h"
#include "../language/regex.h"

#include "../language/hashmap.h"


using shape_type_t = char;
using shape_meta_t = std::vector<std::vector<bool>>;

const shape_meta_t& get_shape_meta(shape_type_t t) {
    switch (t) {
    // TODO:
    // tempalte <char NAME> // constrained in the range {'Q', 'Z', 'S', 'T', 'I', ...}
    // struct shape_t {
    //      constexpr static char name = NAME;
    // };
    case 'Q':
        {
            static shape_meta_t s_meta_Q = {
                {true, true},
                {true, true},
            };
            return s_meta_Q;
        }
    case 'Z':
        {
            static shape_meta_t s_meta_Z = {
                {true, true, false},
                {false, true, true},
            };
            return s_meta_Z;
        }
    case 'S':
        {
            static shape_meta_t s_meta_S = {
                {false, true, true},
                {true, true, false},
            };
            return s_meta_S;
        }
    case 'T':
        {
            static shape_meta_t s_meta_T = {
                {true, true, true},
                {false, true, false},
            };
            return s_meta_T;
        }
    case 'I':
        {
            static shape_meta_t s_meta_I = {
                {true, true, true, true},
            };
            return s_meta_I;
        }
    case 'L':
        {
            static shape_meta_t s_meta_L = {
                {true, false},
                {true, false},
                {true, true},
            };
            return s_meta_L;
        }
    case 'J':
        {
            static shape_meta_t s_meta_J = {
                {false, true},
                {false, true},
                {true, true},
            };
            return s_meta_J;
        }
    default:
        std::cout << "Impossible, should not reach here." << std::endl;
        exit(1);
    }
};

struct input_data_t {
    shape_type_t shape = '\0';
    int starting_col = -1;
};

std::ostream& operator<<(std::ostream& os, const input_data_t& data) {
    os << "shape=" << data.shape << "|starting_col=" << data.starting_col;
    return os;
}

class input_reader_t final {
public:
    explicit input_reader_t(std::string_view file) : is_{ifs_} {
        if (ifs_.open(file.data()); !ifs_.is_open()) {
            std::cout << "Error: failed to open file " << file << std::endl;
            exit(1);
        }
    }
    explicit input_reader_t(std::istream& ist) : is_{ist} {
    }
    bool read_line(std::vector<input_data_t>& data_series) {
/*
        std::vector<std::pair<std::string, int>> all = {
            {"I0,I4,Q8", 1},
            {"T1,Z3,I4", 4},
            {"Q0,I2,I6", 1},
            {"Q0,I2,I6,I0,I6,I6", 2},
            {"Q0,I2,I6,I0,I6,I6,Q2", 4},
            {"Q0,I2,I6,I0,I6,I6,Q2,Q4", 3},

            {"Q0", 2},
            {"Q0,Q1", 4},
            {"Q0,Q2,Q4,Q6,Q8", 0},
            {"Q0,Q2,Q4,Q6,Q8,Q1", 2},
            {"Q0,Q2,Q4,Q6,Q8,Q1,Q1", 4},
            {"I0,I4,Q8", 1},
            {"I0,I4,Q8,I0,I4", 0},
            {"L0,J2,L4,J6,Q8", 2},
            {"L0,Z1,Z3,Z5,Z7", 2},
            {"T0,T3", 2},
            {"T0,T3,I6,I6", 1},
            {"I0,I6,S4", 1},
            {"T1,Z3,I4", 4},
            {"L0,J3,L5,J8,T1", 3},
            {"L0,J3,L5,J8,T1,T6", 1},
            {"L0,J3,L5,J8,T1,T6,J2,L6,T0,T7", 2},
            {"L0,J3,L5,J8,T1,T6,J2,L6,T0,T7,Q4", 1},
            {"S0,S2,S4,S6", 8},
            {"S0,S2,S4,S5,Q8,Q8,Q8,Q8,T1,Q1,I0,Q4", 8},
            {"L0,J3,L5,J8,T1,T6,S2,Z5,T0,T7", 0},
            {"Q0,I2,I6,I0,I6,I6,Q2,Q4", 3},
        };
        static int index = 0;
        if (index == all.size()) return false;
        const auto& sample = all[index++];
        auto input = sample.first;

        std::cout << "Input=" << input << ", height=" << sample.second << std::endl;
*/
        data_series.clear();
        if (!is_.good()) {
            return false;
        }

        std::string input;
        std::getline(is_, input);
        if (is_.fail()) {
            return false;
        }

        auto trim = [](std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](auto v) {
                return !std::isspace(v);
            }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [](auto v) {
                return !std::isspace(v);
            }).base(), s.end());
        };

        trim(input);

        if (input.empty()) {
            return false;
        }

        std::cout << "Input=" << input << std::endl;
        
        auto split = [](std::string_view str, char delimiter) {
            size_t last = 0;
            size_t next = 0;
            std::vector<std::string> tokens;
            while ((next = str.find(delimiter, last)) != string::npos) {
                tokens.emplace_back(str.substr(last, next-last));
                last = next + 1;
            }
            tokens.emplace_back(str.substr(last));
            return tokens;
        };

        auto error = [](std::string_view line, std::string_view token) {
            std::cout << "Error: invalid input line=" << line << ", token=" << token << std::endl;
        };


        const auto tokens = split(input, ',');
        for (const auto& token : tokens) {
            if (token.size() < 2) {
                error(input, token);
                return false;
            }

            const auto shape = token[0];
            if (shape != 'Q' && shape != 'Z' && shape != 'S' && shape != 'T' &&
                shape != 'I' && shape != 'L' && shape != 'J') {
                error(input, token);
                return false;
            }

            std::size_t pos = 0;
            const auto starting_col = std::stoi(token.substr(1), &pos);
            if (pos != token.size() - 1) {
                error(input, token);
                return false;
            }

            data_series.emplace_back(input_data_t{
                .shape = shape,
                .starting_col = starting_col,
            });
        }

        return true;
    }

private:
    std::ifstream ifs_;
    std::istream& is_;
};

class teris_engine_t {
public:
    explicit teris_engine_t(int max_width, int max_height) : max_width_{max_width}, max_height_{max_height} {
    }

    teris_engine_t& set_reader(std::shared_ptr<input_reader_t> reader) {
        reader_ = reader;
        return *this;
    }

    void run() {
        if (!reader_) {
            std::cout << "Error: invalid reader, no idea where the input data is coming from." << std::endl;
            return;
        }

        std::cout << "\n--------------------------------------------" << std::endl;

        std::vector<input_data_t> input_sequence;
        while (reader_->read_line(input_sequence)) {

            internal_state_t state{max_width_, max_height_};

            for (const auto& input : input_sequence) {

                std::cout << "\n Adding input shape: " << input << std::endl;
                
                if (!validate_boundary(input)) {
                    std::cout << "Error: input piece is out of boundary, '" << input << "'" << std::endl;
                    return;
                }

                // add_shape: find starting row - the location where shape's left-bottom cell sits
                const auto starting_row = find_starting_row(input, state);
                std::cout << "\t\t starting_row=" << starting_row << std::endl;

                // add shape to the grid, update grid
                const auto min_idx_updated_row = update_grid(input, state, starting_row);

                // erase_rows();
                auto erased = erase_rows(state, min_idx_updated_row);
                if (!erased.empty()) {
                    std::cout << "\t\t erased=[ ";
                    for (auto e : erased) {
                        std::cout << e << " ";
                    }
                    std::cout << "]" << std::endl;
                }
            }

            std::cout << "\nGrid height=" << state.grid_height << std::endl;

            print(state);

            std::cout << "\n--------------------------------------------" << std::endl;
        }
    }

private:
    struct internal_state_t {
        internal_state_t(int max_width, int max_height)
            : grid(max_height, std::vector<bool>(max_width, false))
            , occupied_by_rows(max_height, 0)
            , height_by_cols(max_width, 0)
        {
        }
        int grid_height{0};

        // row * col, cell value 0/false means no occupied, 1/true means occupied
        std::vector<std::vector<bool>> grid;

        // for each row, track the number of occupied cells
        std::vector<int> occupied_by_rows;

        // for each column, track the height
        std::vector<int> height_by_cols;
    };

    bool validate_boundary(const input_data_t& input) {
        const auto& shape_meta = get_shape_meta(input.shape);
        const auto shape_width = static_cast<int>(shape_meta.front().size());
        return (input.starting_col >= 0 && input.starting_col + shape_width <= max_width_);
    };

    // add_shape: find starting row - the location where shape's left-bottom cell sits
    int find_starting_row(const input_data_t& input, const internal_state_t& state) {
        int starting_row = -1;
        int starting_col = input.starting_col;

        const auto& shape_meta = get_shape_meta(input.shape);

        // find the location
        int row_depth = 0;
        int idx_shape_row = static_cast<int>(shape_meta.size());
        while (idx_shape_row--) {
            int max_possible_row = -1;
            int idx_shape_col = static_cast<int>(shape_meta[idx_shape_row].size());

            while (idx_shape_col--) {
                const bool occupied = shape_meta[idx_shape_row][idx_shape_col];
                if (!occupied) {
                    continue;
                }
                int idx_grid_col = starting_col + idx_shape_col;
                int idx_grid_row = state.height_by_cols[idx_grid_col];
                while (idx_grid_col < max_width_ && idx_grid_row >= 0 && !state.grid[idx_grid_row][idx_grid_col]) {
                    --idx_grid_row;
                }
                max_possible_row = std::max(max_possible_row, idx_grid_row + 1);
            }

            starting_row = std::max(starting_row, max_possible_row - row_depth);

            ++row_depth;
        }
        return starting_row;
    }

    int update_grid(const input_data_t& input, internal_state_t& state, int starting_row) {
        int min_idx_updated_row = max_height_;

        const auto& shape_meta = get_shape_meta(input.shape);

        int row_depth = 0;
        int idx_shape_row = static_cast<int>(shape_meta.size());
        while (idx_shape_row--) {
            int idx_shape_col = static_cast<int>(shape_meta[idx_shape_row].size());

            while (idx_shape_col--) {
                const bool occupied = shape_meta[idx_shape_row][idx_shape_col];
                // if the cell in the shape is not occupied, do nothing
                if (!occupied) {
                    continue;
                }

                // mark the cell occupied in the grid
                int idx_grid_row = starting_row + row_depth;
                int idx_grid_col = input.starting_col + idx_shape_col;
                state.grid[idx_grid_row][idx_grid_col] = occupied;

                // update height of the column, height is always (idx_grid_row + 1)
                const int height_by_col = idx_grid_row + 1;
                state.height_by_cols[idx_grid_col] = height_by_col;
                state.grid_height = std::max(state.grid_height, height_by_col);

                // update (increased by 1) the number of occupied cells in the row
                if (++state.occupied_by_rows[idx_grid_row] == max_width_ && min_idx_updated_row == max_height_) {
                    // track the starting row that is fully occupied to save some loop in following step
                    min_idx_updated_row = idx_grid_row;
                }
            }
            ++row_depth;
        }

        return min_idx_updated_row;
    };

    std::set<int> erase_rows(internal_state_t& state, int min_idx_updated_row) {
        std::set<int> erased;

        if (min_idx_updated_row == max_height_) {
            return erased;
        }

        int num_erased_rows = 0;
        auto idx_grid_row = min_idx_updated_row;
        while (true) {
            const auto occupied = state.occupied_by_rows[idx_grid_row];
            if (occupied == 0) {
                break;
            }

            if (occupied == max_width_) {
                // reset if this row is full
                state.occupied_by_rows[idx_grid_row] = 0;
                state.grid[idx_grid_row] = std::vector<bool>(max_width_, false);

                erased.emplace(idx_grid_row);
                ++num_erased_rows;
                ++idx_grid_row;
                continue;
            }

            const auto idx_grid_new_row = idx_grid_row - num_erased_rows;
            std::swap(state.grid[idx_grid_new_row], state.grid[idx_grid_row]);
            std::swap(state.occupied_by_rows[idx_grid_new_row], state.occupied_by_rows[idx_grid_row]);

            ++idx_grid_row;
        }

        for (auto& height : state.height_by_cols) {
            height -= num_erased_rows;
        }

        state.grid_height -= num_erased_rows;

        return erased;
    }

    void print (const internal_state_t& state) {
        std::ostringstream oss;

        oss << std::setw(3) << "";
        for (auto height : state.height_by_cols) {
            oss << std::setw(3) << height;
        }
        oss << "\n";;

        bool touched_top = false;
        int rows = static_cast<int>(state.occupied_by_rows.size());
        while (rows--) {
            const auto count_occupied = state.occupied_by_rows[rows];
            if (count_occupied == 0 && !touched_top) {
                continue;
            }
            touched_top = true;

            oss << std::setw(3) << count_occupied;

            for (auto occupied : state.grid[rows]) {
                oss << std::setw(3) << (occupied ? 'x' : ' ');
            }

            oss << "\n";
        }
        std::cout << oss.str() << std::endl;
    }


    const int max_width_ = -1;
    const int max_height_ = -1; // TODO: this is not yet implemented
    std::shared_ptr<input_reader_t> reader_;
};

int main(int argc, char **argv) {
    std::shared_ptr<input_reader_t> reader;
    if (argc > 1) {
        std::cout << "Input file: " << argv[1] << std::endl;
        reader = std::make_shared<input_reader_t>(argv[1]);
    } else {
        std::cout << "Waiting for your input: empty input line, Ctrl+D, and Ctrl+Z will exit the app..." << std::endl;
        reader = std::make_shared<input_reader_t>(std::cin);
    }

    constexpr int Max_Grid_Width = 10;
    constexpr int Max_Grid_Height = 200;

    teris_engine_t engine{Max_Grid_Width, Max_Grid_Height};
    engine.set_reader(reader);

    engine.run();

    return 0;


    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

