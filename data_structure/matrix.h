#pragma once

#include "../core/header.h"
#include <new>

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
#ifdef __APPLE__
        ::operator delete(data_);
#else
        std::free(data_);
#endif
    }
    matrix_t(const matrix_t& other)
    {
        // ...
    }
    matrix_t& operator=(const matrix_t& other)
    {
        // ...
        return *this;
    }
    matrix_t(const matrix_t&& other)
    {
        // ...
    }
    matrix_t& operator=(const matrix_t&& other)
    {
        // ...
        return *this;
    }

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
    matrix_t<T> multiply(const matrix_t<T>& other) const
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
    matrix_t<T> multiply_2(const matrix_t<T>& other) const
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

private:
    const size_t rows_ = 0;
    const size_t cols_ = 0;
    T*           data_ = nullptr;
};

