#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//
// This file contains implementation of a simple teris game engine and main() function for usage demonstration.
//
// Compile (verified under ubuntu 24.04, g++ version 13.2):
//      > g++ -std=c++17 teris.cpp -o teris_exe
//
// Usage:
//      1) Read data from input file.
//          > ./teris_exe /absolute_path_input_file
//      2) Read data from STDIN.
//          > ./teris_exe
//
// Sample:
//      > ./teris_exe
//      Waiting for your input: empty input line, Ctrl+D, or Ctrl+Z will exit the app...
//      Q0,Q1                           --> your input
//      4
//      I0,I4,Q8,I0,I4                  --> your input
//      0
//
//      >./teris_exe /tmp/input.txt     --> the given input.txt
//      2
//      4
//      0
//      2
//      4
//      1
//      0
//      2
//      2
//      2
//      1
//      1
//      4
//      3
//      1
//      2
//      1
//      8
//      8
//      0
//      3
//
// Implementation highlights:
// 0) For simplicity, all codes are in one file. Hopefully the naming convention will help you know the code, though some comments are available.
// 1) input_shape_t: a structure that represents individual piece/shape from user.
// 2) get_shape_meta: a simple but important utility that tells each shape's meta data. Meta data is basically a matrix that tells occupied/non-occupied cells in the shape. Any time when we want to add a new shape to the game, just add its meta to this utility so that engine will pick it up automatically.
// 3) input_reader_t: a class that processes input data from either file or standard concole.
// 4) teris_engine_t: a generic game engine class.
// 5) internal_state_t: consists of a few of containers that store the game state per run.
//

// A structure that represents individual piece/shape from user.
struct input_shape_t {
    char shape = '\0';
    int starting_col = -1;
};

inline std::ostream& operator<<(std::ostream& os, const input_shape_t& data) {
    os << "shape=" << data.shape << "|starting_col=" << data.starting_col;
    return os;
}

// A utility function that tells each shape's meta data.
// What is meta data? Meta data is basically a matrix/rectangle that tells occupied/non-occupied cells in the shape.
// Any time when a new shape is created, we just need to add its meta to this utility so that engine will pick it up automatically.
const auto& get_shape_meta(char t) {
    using shape_meta_t = std::vector<std::vector<bool>>;
    switch (t) {
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

// A class that processes input data from either file or standard concole.
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

    // Read & parse a line from input, return true if succeed, false otherwise.
    bool read_line(std::vector<input_shape_t>& shape_series) {
        shape_series.clear();
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

        //std::cout << " Input=" << input << std::endl;
        
        auto split = [](std::string_view str, char delimiter) {
            size_t last = 0;
            size_t next = 0;
            std::vector<std::string> tokens;
            while ((next = str.find(delimiter, last)) != std::string::npos) {
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

            shape_series.emplace_back(input_shape_t{
                .shape = shape,
                .starting_col = starting_col,
            });
        }

        return true;
    }

private:
    std::istream& is_;
    std::ifstream ifs_;
};

// A generic game engine class that is transparent to individual shape but cares of the meta data only.
class teris_engine_t {
public:
    explicit teris_engine_t(int max_width, int max_height) : max_width_{max_width}, max_height_{max_height} {
    }

    teris_engine_t& set_reader(std::unique_ptr<input_reader_t> reader) {
        reader_ = std::move(reader);
        return *this;
    }

    void run() {
        if (!reader_) {
            std::cout << "Error: invalid reader, no idea where the input data is coming from." << std::endl;
            return;
        }

        std::vector<input_shape_t> input_sequence;
        while (reader_->read_line(input_sequence)) {

            internal_state_t state{max_width_, max_height_};

            for (const auto& input : input_sequence) {
                if (!validate_boundary(input)) {
                    std::cout << "Error: input shape is out of boundary, '" << input << "'" << std::endl;
                    return;
                }

                // add_shape: find starting row - the location where shape's left-bottom cell sits
                const auto starting_row = find_starting_row(input, state);

                // add shape to the grid, update grid
                const auto min_idx_updated_row = update_grid(input, state, starting_row);

                erase_rows(state, min_idx_updated_row);
            }

            //std::cout << " Grid height=" << state.grid_height << std::endl;
            std::cout << state.grid_height << std::endl;
        }
    }

private:
    // IMPORTANT: track the game state per run.
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

    // Check if the input is valid, i.e. out of grid boundary.
    bool validate_boundary(const input_shape_t& input) {
        const auto& shape_meta = get_shape_meta(input.shape);
        const auto shape_width = static_cast<int>(shape_meta.front().size());
        return (input.starting_col >= 0 && input.starting_col + shape_width <= max_width_);
    };

    // For a coming input shape, find its starting row in the grid: the location where shape's left-bottom cell sits.
    int find_starting_row(const input_shape_t& input, const internal_state_t& state) {
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

    // Given a coming input shape's location in the grid, update internal state.
    // Return the minimal row index that is fully occupied (to be erased later).
    int update_grid(const input_shape_t& input, internal_state_t& state, int starting_row) {
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

    // Erase the fully occupied row(s) in the grid, and update the state.
    // Return the number of erased rows.
    int erase_rows(internal_state_t& state, int min_idx_updated_row) {
        if (min_idx_updated_row == max_height_) {
            return 0;
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

        return num_erased_rows;
    }

private:
    const int max_width_ = -1;
    const int max_height_ = -1; // TODO: this is not yet implemented
    std::unique_ptr<input_reader_t> reader_;
};

int main(int argc, char **argv) {
    std::unique_ptr<input_reader_t> reader;
    if (argc > 1) {
        reader = std::make_unique<input_reader_t>(argv[1]);
    } else {
        std::cout << "Waiting for your input: empty input line, Ctrl+D, or Ctrl+Z will exit the app..." << std::endl;
        reader = std::make_unique<input_reader_t>(std::cin);
    }

    constexpr int Max_Grid_Width = 10;
    constexpr int Max_Grid_Height = 200;

    teris_engine_t engine{Max_Grid_Width, Max_Grid_Height};
    engine.set_reader(std::move(reader)).run();

    return 0;
}

