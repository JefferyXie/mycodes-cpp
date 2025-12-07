#include <iostream>
#include <cstring>

template <class T>
class Matrix
{
private:
    int m_rows;
    int m_cols;
    T** m_pMatrix;

    Matrix() {}

public:
    Matrix(int rows, int cols)
    {
        m_rows = rows;
        m_cols = cols;
        // http://stackoverflow.com/questions/18273370/the-correct-way-to-initialize-a-dynamic-pointer-to-a-multidimensional-array
        // int** q = new int[2][3] {{1,2,3}, {4,5,6}};
        // int* qq[2] = new int[2][3] {{1,2,3}, {4,5,6}};
        // correct way to dynamically create multi-dimentional array
        m_pMatrix = new T*[m_rows];
        for (int i = 0; i < m_rows; ++i) {
            m_pMatrix[i] = new T[cols];
        }
    }
    virtual ~Matrix()
    {
        // correct way to release multi-dimentional array
        for (int i = 0; i < m_rows; ++i) {
            delete[] m_pMatrix[i];
        }
        delete[] m_pMatrix;
    }
    Matrix(const Matrix& matrix)
    {
        // ...
    }
    Matrix& operator=(const Matrix& matrix)
    {
        // ...
        return *this;
    }
    Matrix(const Matrix&& matrix)
    {
        // ...
    }
    Matrix& operator=(const Matrix&& matrix)
    {
        // ...
        return *this;
    }

    // properties
    int Rows() { return m_rows; }
    int Cols() { return m_cols; }

    // methods
    Matrix<T> operator*(const Matrix<T>& matrix) { return Multiply(matrix); }
    Matrix<T> operator+(const Matrix<T>& matrix) { return Add(matrix); }
    Matrix<T> operator-(const Matrix<T>& matrix) { return Minus(matrix); }
    Matrix<T> Multiply(const Matrix<T>& matrix)
    {
        Matrix<T> result(Rows(), matrix.Cols());
        // ...
        return result;
    }
    Matrix<T> Add(const Matrix<T>& matrix)
    {
        Matrix<T> result(Rows(), Cols());
        // ...
        return result;
    }
    Matrix<T> Minus(const Matrix<T>& matrix)
    {
        Matrix<T> result(Rows(), Cols());
        // ...
        return result;
    }
    Matrix<T> Transpose() { return *this; }
    T&        Get(int row, int col)
    {
        if (row < m_rows && col < m_cols && row >= 0 && col >= 0)
            return m_pMatrix[row][col];
    }
    const T& Get(int row, int col) const
    {
        if (row < m_rows && col < m_cols && row >= 0 && col >= 0)
            return m_pMatrix[row][col];
    }
    T*& operator[](int row)
    {
        if (row < m_rows && row >= 0)
            return m_pMatrix[row];
    }
    T&       operator()(int row, int col) { return Get(row, col); }
    const T& operator()(int row, int col) const { return Get(row, col); }
};

