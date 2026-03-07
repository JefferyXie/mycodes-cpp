#pragma once

#include "../core/header.h"

template <typename T>
class matrix_t final
{
public:
    matrix_t() = default;
    matrix_t(size_t rows, size_t cols) : rows_{rows}, cols_{cols}
    {
        const auto alignment = []() {
            auto x = sizeof(T);
            if ((x & (x - 1)) == 0) {
                return x;
            }

            size_t align = 1;
            while (x) {
                align *= 2;
                x = x >> 1;
            }
            return align;
        }();

#ifdef __APPLE__
        data_ = static_cast<T*>(::operator new(sizeof(T) * rows_ * cols_, std::align_val_t(alignment)));
#else
        data_ = static_cast<T*>(std::aligned_alloc(alignment, sizeof(T) * rows_ * cols_));
#endif
        if (!data_) {
            std::cout << __FUNCTION__ << ": Failed to allocate, alignment=" << alignment
                      << ", size=" << (sizeof(T) * rows_ * cols_) << std::endl;
            throw std::bad_alloc();
        }
        std::fill_n(data_, rows_ * cols_, 0);
    }

    matrix_t(std::initializer_list<std::initializer_list<T>> m)
        : matrix_t{m.size(), m.size() == 0 ? 0 : m.begin()->size()}
    {
        for (size_t r = 0; r < rows_; ++r) {
            auto& data_r = m.begin()[r];
            if (data_r.size() != cols_) {
                throw std::invalid_argument("input matrix is illformed");
            }

            for (size_t c = 0; c < data_r.size(); ++c) {
                (*this)(r, c) = std::move(data_r.begin()[c]);
            }
        }
    }

    matrix_t(const std::vector<std::vector<T>>& m) : matrix_t{m.size(), m.empty() ? 0 : m.front().size()}
    {
        for (size_t r = 0; r < rows_; ++r) {
            if (m[r].size() != cols_) {
                throw std::invalid_argument("input matrix is illformed");
            }

            for (size_t c = 0; c < m[r].size(); ++c) {
                (*this)(r, c) = m(r, c);
            }
        }
    }

    ~matrix_t()
    {
        if (data_) {
#ifdef __APPLE__
            ::operator delete(data_);
#else
            std::free(data_);
#endif
        }
    }
    matrix_t(const matrix_t& other) : matrix_t{other.rows(), other.cols()}
    {
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    matrix_t& operator=(const matrix_t& other)
    {
        if (this == &other) {
            return *this;
        }

        if (rows_ == other.rows() && cols_ == other.cols()) {
            for (size_t i = 0; i < rows_ * cols_; ++i) {
                data_[i] = other.data_[i];
            }
        } else {
            swap(matrix_t{other});
        }
        return *this;
    }
    matrix_t(matrix_t&& other) { swap(other); }
    matrix_t& operator=(matrix_t&& other) { swap(other); }

    bool operator==(const matrix_t& other) const
    {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            return false;
        }
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const matrix_t& other) const { return !operator==(other); }

    // properties
    auto rows() const { return rows_; }
    auto cols() const { return cols_; }

    // methods
    matrix_t<T> operator*(const matrix_t<T>& other) const { return multiply(other); }
    matrix_t<T> operator+(const matrix_t<T>& other) const { return add(other); }
    matrix_t<T> operator-(const matrix_t<T>& other) const { return minus(other); }

    // NOTICE: check below video for 10+ optimizations: https://www.youtube.com/watch?v=GHctcSBd6Z4&t=3363s
    matrix_t<T> multiply_basic(const matrix_t<T>& other) const
    {
        matrix_t<T> result(rows(), other.cols());

        // 1) this matrix_t's row is fixed
        for (size_t r = 0; r < rows_; ++r) {
            // 2) loop other matrix_t's col
            for (size_t other_c = 0; other_c < other.cols_; ++other_c) {

                // result_cell is fixed at this point
                auto& result_cell = result.data_[r * other.cols_ + other_c];

                // 3) loop each col cells on this matrix_t's current row
                for (size_t c = 0; c < cols_; ++c) {
                    // this_cell keeps changing, though the memory is continuous
                    const auto this_cell = data_[r * cols_ + c];

                    // other_cell keeps changing and memory is certainly NOT continuous
                    //
                    // NOTICE: this is the main different from above version and causes slowness, though the result_cell
                    //         in this case is fixed but that won't compliment the cost by other_cell!!
                    const auto other_cell = other.data_[c * other.cols_ + other_c];

                    result_cell += this_cell * other_cell;
                }
            }
        }
        return result;
    }

    matrix_t<T> multiply_order(const matrix_t<T>& other) const
    {
        matrix_t<T> result(rows(), other.cols());

        // 1) this matrix_t's row is fixed
        for (size_t r = 0; r < rows_; ++r) {
            // 2) loop this matrix_t's col
            for (size_t c = 0; c < cols_; ++c) {

                // this_cell is fixed at this point
                const auto this_cell = data_[r * cols_ + c];

                // 3) loop other matrix_t's col
                for (size_t other_c = 0; other_c < other.cols_; ++other_c) {
                    // other_cell keeps changing, but memory is continuous
                    const auto other_cell = other.data_[c * other.cols_ + other_c];

                    // result_cell keeps changing, but memory is continuous
                    auto& result_cell = result.data_[r * other.cols_ + other_c];

                    result_cell += this_cell * other_cell;
                }
            }
        }
        return result;
    }

    // https://alvinwan.com/how-to-tile-matrix-multiplication/
    // Tiling enhances matrix multiplication performance by partitioning large matrices into smaller blocks that fit
    // into the CPU cache, thereby maximizing data reuse and reducing memory bandwidth bottlenecks.
    matrix_t<T> multiply_tiling(const matrix_t<T>& other) const
    {
        matrix_t<T> result(rows(), other.cols());

        const auto cols_this  = (int)cols_;
        const auto cols_other = (int)other.cols();

        auto impl = [cols_this, cols_other](
                        auto impl_rows, auto impl_cols, auto impl_cols_other, auto* p_this, auto* p_other,
                        auto* p_result) {
            for (size_t r = 0; r < impl_rows; ++r, p_this += cols_this, p_result += cols_other) {
                const auto* p_other_cur = p_other;
                for (size_t c = 0; c < impl_cols; ++c, p_other_cur += cols_other) {
                    for (size_t other_c = 0; other_c < impl_cols_other; ++other_c) {
                        p_result[other_c] += p_this[c] * p_other_cur[other_c];
                    }
                }
            }
        };

        // TODO: rough estimation, may not be acurate, sqrt(L1 cache / sizeof(T)) / total 3 arrays
        // constexpr size_t BLOCK = std::sqrt(1024 * 256 / sizeof(T)) / 3;
        constexpr size_t BLOCK = 32;

        for (size_t r = 0; r < rows_; r += BLOCK) {
            for (size_t c = 0; c < cols_; c += BLOCK) {
                for (size_t other_c = 0; other_c < other.cols_; other_c += BLOCK) {
                    auto* p_this   = &data_[r * cols_ + c];
                    auto* p_other  = &other.data_[c * other.cols_ + other_c];
                    auto* p_result = &result.data_[r * other.cols_ + other_c];

                    const auto impl_rows       = std::min(BLOCK, rows_ - r);
                    const auto impl_cols       = std::min(BLOCK, cols_ - c);
                    const auto impl_cols_other = std::min(BLOCK, cols_other - other_c);

                    impl(impl_rows, impl_cols, impl_cols_other, p_this, p_other, p_result);
                }
            }
        }

        return result;
    }

    // cache aware implementation: consider both L1 cache and registers
    matrix_t<T> multiply_cache(const matrix_t<T>& other) const
    {
        matrix_t<T> result(rows(), other.cols());

        const size_t cols_this  = cols_;
        const size_t cols_other = other.cols();

        // size of L1 cache blocks
        constexpr size_t CACHE_rows       = 64;    // 180;
        constexpr size_t CACHE_cols       = 96;    // 240;
        constexpr size_t CACHE_cols_other = 32;    // 96;

        // size of register blocks
        constexpr size_t REG_cols_other = 8;    // 12;
        constexpr size_t REG_rows       = 4;    // 4

        auto impl = [cols_this, cols_other](
                        auto impl_rows, auto impl_cols, auto impl_cols_other, auto* p_this, auto* p_other,
                        auto* p_result) {
            for (size_t r = 0; r < impl_rows; ++r, p_this += cols_this, p_result += cols_other) {    // REG_rows
                const auto* p_other_cur = p_other;
                for (size_t c = 0; c < impl_cols; ++c, p_other_cur += cols_other) {     // CACHE_cols
                    for (size_t other_c = 0; other_c < impl_cols_other; ++other_c) {    // REG_cols_other
                        p_result[other_c] += p_this[c] * p_other_cur[other_c];
                    }
                }
            }
        };

        for (size_t r = 0; r < rows_; r += CACHE_rows) {                                          // CACHE_rows
            for (size_t c = 0; c < cols_; c += CACHE_cols) {                                      // CACHE_cols
                for (size_t other_c = 0; other_c < other.cols_; other_c += CACHE_cols_other) {    // CACHE_cols_other
                    auto* p_this   = &data_[r * cols_ + c];
                    auto* p_other  = &other.data_[c * other.cols_ + other_c];
                    auto* p_result = &result.data_[r * other.cols_ + other_c];

                    const auto rows2       = std::min(CACHE_rows, rows_ - r);
                    const auto cols2       = std::min(CACHE_cols, cols_ - c);
                    const auto cols_other2 = std::min(CACHE_cols_other, cols_other - other_c);

                    // extra layer that consider registers
                    for (size_t r2 = 0; r2 < rows2; r2 += REG_rows) {
                        for (size_t other_c2 = 0; other_c2 < cols_other2; other_c2 += REG_cols_other) {
                            auto* p_this2   = &p_this[r2 * cols_];
                            auto* p_other2  = &p_other[other_c2];
                            auto* p_result2 = &p_result[r2 * cols_other + other_c2];

                            const auto impl_rows       = std::min(REG_rows, rows2 - r2);
                            const auto impl_cols       = cols2;
                            const auto impl_cols_other = std::min(REG_cols_other, cols_other2 - other_c2);

                            impl(impl_rows, impl_cols, impl_cols_other, p_this2, p_other2, p_result2);
                        }
                    }
                }
            }
        }

        return result;
    }

    matrix_t<T> add(const matrix_t<T>& other) const
    {
        matrix_t<T> result(rows(), cols());
        // ...
        return result;
    }
    matrix_t<T> minus(const matrix_t<T>& other) const
    {
        matrix_t<T> result(rows(), cols());
        // ...
        return result;
    }
    matrix_t<T> transpose() const { return *this; }
    T&       get(size_t row, size_t col) { return const_cast<T&>(static_cast<const matrix_t*>(this)->get(row, col)); }
    const T& get(size_t row, size_t col) const
    {
        if (row < rows_ && col < cols_)
            return data_[row * cols_ + col];
        throw std::invalid_argument("row or col is out of range");
    }
    T&       operator()(size_t row, size_t col) { return get(row, col); }
    const T& operator()(size_t row, size_t col) const { return get(row, col); }

    matrix_t& swap(matrix_t& other)
    {
        if (this != &other) {
            std::swap(rows_, other.rows_);
            std::swap(cols_, other.cols_);
            std::swap(data_, other.data_);
        }
        return *this;
    }

    std::string to_string(bool one_row_per_line = false) const
    {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < rows_; ++i) {
            if (one_row_per_line) {
                oss << "\n ";
            }
            oss << "[";
            for (size_t j = 0; j < cols_; ++j) {
                oss << get(i, j);
                if (j != cols_ - 1) {
                    oss << ",";
                }
            }
            oss << "]";
            if (i != rows_ - 1) {
                oss << ",";
            }
        }
        if (one_row_per_line) {
            oss << "\n";
        }
        oss << "]";
        return oss.str();
    }

private:
    size_t rows_ = 0;
    size_t cols_ = 0;
    T*     data_ = nullptr;
};

