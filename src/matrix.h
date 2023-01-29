#pragma once

#include "Types/column.h"
#include "Types/row.h"

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

template <typename T>
class Matrix;

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix);

template <typename T>
class Matrix
{
private:
    Column m_Columns{}; // Colums
    Row m_Rows{};       // Rows
    std::vector<std::vector<T>> m_Matrix{};

    Matrix() noexcept = default;
    explicit Matrix(const Row &rows, const Column &cols);
    explicit Matrix(const std::vector<T> &array);
    explicit Matrix(const std::vector<std::vector<T>> &input_matrix);

public:
    Matrix(const Matrix<T> &) noexcept = default;
    Matrix(Matrix<T> &&) noexcept = default;
    Matrix &operator=(const Matrix<T> &) noexcept = default;
    Matrix &operator=(Matrix<T> &&) noexcept = default;

    static Matrix<T> create();
    static Matrix<T> create(const Row &rows, const Column &cols);
    static Matrix<T> create(const std::vector<T> &array);
    static Matrix<T> create(const std::vector<std::vector<T>> &input_matrix);
    static Matrix<T> create(const Matrix<T> &org);

    const std::vector<std::vector<T>> &getMatrix() const noexcept;
    std::vector<std::vector<T>> &getMatrix() noexcept;
    constexpr const Row &getRows() const noexcept;
    constexpr const Column &getCols() const noexcept;
    std::string toString() const noexcept;

    void erase() noexcept;

    bool transpose();

    friend std::ostream &operator<< <>(std::ostream &os, const Matrix<T> &matrix);

    Matrix<T> &operator+=(const Matrix<T> &org);
    Matrix<T> &operator+=(const T &n);
    Matrix<T> &operator-=(const Matrix<T> &org);
    Matrix<T> &operator-=(const T &n);
    Matrix<T> &operator*=(const Matrix<T> &org);
    Matrix<T> &operator*=(const T &n);

    Matrix<T> operator+(const Matrix<T> &org);
    Matrix<T> operator+(const T &n);
    Matrix<T> operator-(const Matrix<T> &org);
    Matrix<T> operator-(const T &n);
    Matrix<T> operator*(const Matrix<T> &org);
    Matrix<T> operator*(const T &n);

    bool operator==(const Matrix<T> &org);
    bool operator!=(const Matrix<T> &org);
};

template <typename T>
Matrix<T>::Matrix(const Row &rows, const Column &cols)
{
    this->m_Columns = cols;
    this->m_Rows = rows;
    if (this->m_Rows > 0UL)
    {
        this->m_Matrix.resize(m_Rows.get());
        if (this->m_Columns > 0UL)
        {
            for (auto &&column : this->m_Matrix)
            {
                column.resize(cols.get());
            }
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const std::vector<T> &array)
{
    this->m_Columns = 1;
    this->m_Rows = array.size();

    this->m_Matrix.resize(this->m_Rows.get());
    for (size_t i = 0; i < this->m_Rows; i++)
    {
        this->m_Matrix.at(i).push_back(array.at(i));
    }
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &input_matrix)
{
    this->m_Rows = input_matrix.size();
    if (this->m_Rows > 0UL)
        this->m_Columns = input_matrix.at(0).size();
    else
        this->m_Columns = 0;

    this->m_Matrix = input_matrix;
}

template <typename T>
inline Matrix<T> Matrix<T>::create()
{
    return Matrix<T>();
}
template <typename T>
inline Matrix<T> Matrix<T>::create(const Row &rows, const Column &cols)
{
    return Matrix<T>(rows, cols);
}

template <typename T>
inline Matrix<T> Matrix<T>::create(const std::vector<T> &array)
{
    return Matrix<T>(array);
}

template <typename T>
inline Matrix<T> Matrix<T>::create(const std::vector<std::vector<T>> &input_matrix)
{
    return Matrix<T>(input_matrix);
}

template <typename T>
inline Matrix<T> Matrix<T>::create(const Matrix<T> &org)
{
    return Matrix<T>(org);
}

template <typename T>
inline const std::vector<std::vector<T>> &Matrix<T>::getMatrix(void) const noexcept
{
    return this->m_Matrix;
}

template <typename T>
inline std::vector<std::vector<T>> &Matrix<T>::getMatrix() noexcept
{
    return this->m_Matrix;
}

template <typename T>
constexpr const Row &Matrix<T>::getRows() const noexcept
{
    return this->m_Rows;
}

template <typename T>
constexpr const Column &Matrix<T>::getCols() const noexcept
{
    return this->m_Columns;
}

template <typename T>
inline std::string Matrix<T>::toString() const noexcept
{
    std::string result = "";
    for (auto &&column : this->m_Matrix)
    {
        result += " | ";
        for (auto &&element : column)
        {
            result += std::to_string(element);
            result += " | ";
        }
        result += "\n";
    }
    return result;
}

template <typename T>
inline bool Matrix<T>::transpose()
{
    std::vector<std::vector<T>> result = this->getMatrix();

    Matrix<T> newMatrix = create(Row{this->m_Columns.get()}, Column{this->m_Rows.get()});
    this->m_Matrix = newMatrix.m_Matrix;

    for (size_t i = 0; i < this->m_Columns; i++)
    {
        for (size_t j = 0; j < this->m_Rows; j++)
        {
            this->m_Matrix.at(i).at(j) = result.at(j).at(i);
        }
    }

    Row temp{this->m_Rows};
    this->m_Rows = this->m_Columns.get();
    this->m_Columns = temp.get();

    return true;
}

template <typename T>
inline void Matrix<T>::erase() noexcept
{
    this->m_Matrix.clear();
    this->m_Columns = 0UL;
    this->m_Rows = 0UL;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
{
    for (const auto &column : matrix.getMatrix())
    {
        os << " | ";
        for (const auto &element : column)
        {
            os << element << " | ";
        }
        os << "\n";
    }
    return os;
}

template <typename T>
inline Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &org)
{
    auto tResult = *this + org;
    *this = std::move(tResult);

    return *this;
}
template <typename T>
inline Matrix<T> &Matrix<T>::operator+=(const T &n)
{
    auto tResult = *this + n;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
inline Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &org)
{
    auto tResult = *this - org;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
inline Matrix<T> &Matrix<T>::operator-=(const T &n)
{
    auto tResult = *this - n;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
inline Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &org)
{
    auto tResult = *this * org;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
inline Matrix<T> &Matrix<T>::operator*=(const T &n)
{
    auto tResult = *this * n;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator+(const Matrix<T> &org)
{
    if (this->m_Rows.get() == org.m_Rows.get() || this->m_Columns.get() == org.m_Columns.get())
    {
        Matrix<T> result = create(this->m_Rows, this->m_Columns);

        for (size_t i = 0; i < this->m_Matrix.size(); i++)
        {
            for (size_t j = 0; j < this->m_Matrix.at(i).size(); j++)
            {
                result.m_Matrix.at(i).at(j) = this->m_Matrix.at(i).at(j) + org.m_Matrix.at(i).at(j);
            }
        }
        return result;
    }

    return *this;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator+(const T &n)
{
    Matrix<T> result = create(this->m_Rows, this->m_Columns);
    for (size_t i = 0; i < this->m_Rows; i++)
    {

        for (size_t j = 0; j < this->m_Columns; j++)
        {
            result.m_Matrix.at(i).at(j) = this->m_Matrix.at(i).at(j) + n;
        }
    }

    return result;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator-(const Matrix<T> &org)
{
    if (this->m_Rows.get() == org.m_Rows.get() || this->m_Columns.get() == org.m_Columns.get())
    {
        Matrix<T> result = create(this->m_Rows, this->m_Columns);

        for (size_t i = 0; i < this->m_Matrix.size(); i++)
        {
            for (size_t j = 0; j < this->m_Matrix.at(i).size(); j++)
            {
                result.m_Matrix.at(i).at(j) = this->m_Matrix.at(i).at(j) - org.m_Matrix.at(i).at(j);
            }
        }
        return result;
    }

    return *this;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator-(const T &n)
{
    Matrix<T> result = create(this->m_Rows, this->m_Columns);
    for (size_t i = 0; i < this->m_Rows; i++)
    {

        for (size_t j = 0; j < this->m_Columns; j++)
        {
            result.m_Matrix.at(i).at(j) = this->m_Matrix.at(i).at(j) - n;
        }
    }

    return result;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T> &org)
{
    std::vector<std::vector<T>> result;
    if (this->m_Columns.get() == org.m_Rows.get())
    {
        result.resize(this->m_Rows.get());
        for (size_t i = 0; i < this->m_Rows.get(); i++)
        {
            result.at(i).resize(org.m_Columns.get());
            for (size_t j = 0; j < org.m_Columns.get(); j++)
            {
                T sum = 0;
                for (size_t k = 0; k < this->m_Columns.get(); k++)
                {
                    sum += this->m_Matrix.at(i).at(k) * org.m_Matrix.at(k).at(j);
                }
                result.at(i).at(j) = sum;
            }
        }
    }
    else if (*this == org)
    {
        result.resize(this->m_Rows.get());
        for (size_t i = 0; i < result.size(); i++)
        {
            result.at(i).resize(this->m_Columns.get());
            for (size_t j = 0; j < result.at(i).size(); j++)
            {
                result.at(i).at(j) = this->m_Matrix.at(i).at(j) * this->m_Matrix.at(i).at(j);
            }
        }
    }
    return Matrix<T>{result};
}

template <typename T>
inline Matrix<T> Matrix<T>::operator*(const T &n)
{
    Matrix<T> result = create(this->m_Rows, this->m_Columns);
    for (size_t i = 0; i < this->m_Rows; i++)
    {
        for (size_t j = 0; j < this->m_Columns; j++)
        {
            result.m_Matrix.at(i).at(j) = this->m_Matrix.at(i).at(j) * n;
        }
    }

    return result;
}

template <typename T>
inline bool Matrix<T>::operator==(const Matrix<T> &org)
{
    if (this->m_Columns.get() == org.m_Columns.get() && this->m_Rows.get() == org.m_Rows.get())
    {
        for (size_t i = 0; i < this->m_Rows; i++)
        {
            for (size_t j = 0; j < this->m_Columns; j++)
            {
                if (this->m_Matrix.at(i).at(j) != org.m_Matrix.at(i).at(j))
                    return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
inline bool Matrix<T>::operator!=(const Matrix<T> &org)
{
    return !(*this == org);
}