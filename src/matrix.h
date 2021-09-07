#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>
#include "Types/column.h"
#include "Types/row.h"

template <typename T>
class Matrix;

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix);

template <typename T>
class Matrix
{
private:
    Column m_nCols{}; // Colums
    Row m_nRows{};    // Rows
    std::vector<std::vector<T>> m_aMatrix{};

    Matrix() noexcept = default;
    Matrix(const Row &rows, const Column &cols);
    Matrix(const std::vector<T> &array);
    Matrix(const std::vector<std::vector<T>> &input_matrix);

public:
    Matrix(const Matrix<T> &org);

    static Matrix<T> create();
    static Matrix<T> create(const Row &rows, const Column &cols);
    static Matrix<T> create(const std::vector<T> &array);
    static Matrix<T> create(const std::vector<std::vector<T>> &input_matrix);
    static Matrix<T> create(const Matrix<T> &org);

    std::vector<std::vector<T>> getMatrix(void) const noexcept;
    constexpr const Row &getRows() const noexcept;
    constexpr const Column &getCols() const noexcept;
    bool randomizeWithInts(void);
    const std::string toString() const noexcept;

    void erase() noexcept;

    bool transpose();

    friend std::ostream &operator<<<>(std::ostream &os, const Matrix<T> &matrix);

    Matrix<T> &operator=(const Matrix<T> &org);
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
    this->m_nCols = cols;
    this->m_nRows = rows;
    if (this->m_nRows > 0ul)
    {
        this->m_aMatrix.resize(m_nRows.get());
        if (this->m_nCols > 0ul)
        {
            for (auto &&column : this->m_aMatrix)
            {
                column.resize(cols.get());
            }
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const std::vector<T> &array)
{
    this->m_nCols = 1;
    this->m_nRows = array.size();

    this->m_aMatrix.resize(this->m_nRows.get());
    for (size_t i = 0; i < this->m_nRows; i++)
    {
        this->m_aMatrix.at(i).push_back(array.at(i));
    }
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &input_matrix)
{
    this->m_nRows = input_matrix.size();
    if (this->m_nRows > 0ul)
        this->m_nCols = input_matrix.at(0).size();
    else
        this->m_nCols = 0;

    this->m_aMatrix = input_matrix;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &org)
{
    this->m_nCols = org.m_nCols;
    this->m_nRows = org.m_nRows;
    this->m_aMatrix = org.m_aMatrix;
}

template <typename T>
Matrix<T> Matrix<T>::create()
{
    return Matrix<T>();
}
template <typename T>
Matrix<T> Matrix<T>::create(const Row &rows, const Column &cols)
{
    return Matrix<T>(rows, cols);
}

template <typename T>
Matrix<T> Matrix<T>::create(const std::vector<T> &array)
{
    return Matrix<T>(array);
}

template <typename T>
Matrix<T> Matrix<T>::create(const std::vector<std::vector<T>> &input_matrix)
{
    return Matrix<T>(input_matrix);
}

template <typename T>
Matrix<T> Matrix<T>::create(const Matrix<T> &org)
{
    return Matrix<T>(org);
}

template <typename T>
std::vector<std::vector<T>> Matrix<T>::getMatrix(void) const noexcept
{
    return this->m_aMatrix;
}

template <typename T>
constexpr const Row &Matrix<T>::getRows() const noexcept
{
    return this->m_nRows;
}

template <typename T>
constexpr const Column &Matrix<T>::getCols() const noexcept
{
    return this->m_nCols;
}

template <typename T>
bool Matrix<T>::randomizeWithInts(void)
{
    if constexpr (std::is_arithmetic_v<T>)
    {
        std::srand(static_cast<unsigned int>(time(NULL)));

        for (auto &column : this->m_aMatrix)
        {
            for (auto &element : column)
            {
                element = (static_cast<T>((std::rand()) % 10000000)) / 10000000;
            }
        }
    }
    else
    {
        for (const auto &column : this->m_aMatrix)
        {
            for (const auto &element : column)
            {
                element.randomize();
            }
        }
    }

    return true;
}

template <typename T>
const std::string Matrix<T>::toString() const noexcept
{
    std::string result = "";
    for (auto &&column : this->m_aMatrix)
    {
        result += " | ";
        for (auto &&element : column)
        {
            result += std::to_string(element);
            result += " | ";
        }
        result += "\n";
    }
    return result;
}

template <typename T>
bool Matrix<T>::transpose()
{
    std::vector<std::vector<T>> result = this->getMatrix();

    Matrix<T> newMatrix = create(Row{this->m_nCols.get()}, Column{this->m_nRows.get()});
    this->m_aMatrix = newMatrix.m_aMatrix;

    for (size_t i = 0; i < this->m_nCols; i++)
    {
        for (size_t j = 0; j < this->m_nRows; j++)
        {
            this->m_aMatrix.at(i).at(j) = result.at(j).at(i);
        }
    }

    Row temp{this->m_nRows};
    this->m_nRows = this->m_nCols.get();
    this->m_nCols = temp.get();

    return true;
}

template <typename T>
void Matrix<T>::erase() noexcept
{
    this->m_aMatrix.clear();
    this->m_nCols = 0ul;
    this->m_nRows = 0ul;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
{
    for (const auto &column : matrix.getMatrix())
    {
        os << " | ";
        for (const auto &element : column)
        {
            os << element << " | ";
        }
        os << "\n";
    }
    return os;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &org)
{
    this->m_nCols = org.m_nCols;
    this->m_nRows = org.m_nRows;
    this->m_aMatrix = org.m_aMatrix;

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &org)
{
    auto tResult = *this + org;
    *this = std::move(tResult);

    return *this;
}
template <typename T>
Matrix<T> &Matrix<T>::operator+=(const T &n)
{
    auto tResult = *this + n;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &org)
{
    auto tResult = *this - org;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const T &n)
{
    auto tResult = *this - n;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &org)
{
    auto tResult = *this * org;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const T &n)
{
    auto tResult = *this * n;
    *this = std::move(tResult);

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &org)
{
    if (this->m_nRows.get() == org.m_nRows.get() || this->m_nCols.get() == org.m_nCols.get())
    {
        Matrix<T> result = create(this->m_nRows, this->m_nCols);

        for (size_t i = 0; i < this->m_aMatrix.size(); i++)
        {
            for (size_t j = 0; j < this->m_aMatrix.at(i).size(); j++)
            {
                result.m_aMatrix.at(i).at(j) = this->m_aMatrix.at(i).at(j) + org.m_aMatrix.at(i).at(j);
            }
        }
        return result;
    }

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const T &n)
{
    Matrix<T> result = create(this->m_nRows, this->m_nCols);
    for (size_t i = 0; i < this->m_nRows; i++)
    {

        for (size_t j = 0; j < this->m_nCols; j++)
        {
            result.m_aMatrix.at(i).at(j) = this->m_aMatrix.at(i).at(j) + n;
        }
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &org)
{
    if (this->m_nRows.get() == org.m_nRows.get() || this->m_nCols.get() == org.m_nCols.get())
    {
        Matrix<T> result = create(this->m_nRows, this->m_nCols);

        for (size_t i = 0; i < this->m_aMatrix.size(); i++)
        {
            for (size_t j = 0; j < this->m_aMatrix.at(i).size(); j++)
            {
                result.m_aMatrix.at(i).at(j) = this->m_aMatrix.at(i).at(j) - org.m_aMatrix.at(i).at(j);
            }
        }
        return result;
    }

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const T &n)
{
    Matrix<T> result = create(this->m_nRows, this->m_nCols);
    for (size_t i = 0; i < this->m_nRows; i++)
    {

        for (size_t j = 0; j < this->m_nCols; j++)
        {
            result.m_aMatrix.at(i).at(j) = this->m_aMatrix.at(i).at(j) - n;
        }
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &org)
{
    std::vector<std::vector<T>> result;
    if (this->m_nCols.get() == org.m_nRows.get())
    {
        result.resize(this->m_nRows.get());
        for (size_t i = 0; i < this->m_nRows.get(); i++)
        {
            result.at(i).resize(org.m_nCols.get());
            for (size_t j = 0; j < org.m_nCols.get(); j++)
            {
                T sum = 0;
                for (size_t k = 0; k < this->m_nCols.get(); k++)
                {
                    sum += this->m_aMatrix.at(i).at(k) * org.m_aMatrix.at(k).at(j);
                }
                result.at(i).at(j) = sum;
            }
        }
    }
    else if (*this == org)
    {
        result.resize(this->m_nRows.get());
        for (size_t i = 0; i < result.size(); i++)
        {
            result.at(i).resize(this->m_nCols.get());
            for (size_t j = 0; j < result.at(i).size(); j++)
            {
                result.at(i).at(j) = this->m_aMatrix.at(i).at(j) * this->m_aMatrix.at(i).at(j);
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &n)
{
    Matrix<T> result = create(this->m_nRows, this->m_nCols);
    for (size_t i = 0; i < this->m_nRows; i++)
    {
        for (size_t j = 0; j < this->m_nCols; j++)
        {
            result.m_aMatrix.at(i).at(j) = this->m_aMatrix.at(i).at(j) * n;
        }
    }

    return result;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &org)
{
    if (this->m_nCols.get() == org.m_nCols.get() && this->m_nRows.get() == org.m_nRows.get())
    {
        for (size_t i = 0; i < this->m_nRows; i++)
        {
            for (size_t j = 0; j < this->m_nCols; j++)
            {
                if (this->m_aMatrix.at(i).at(j) != org.m_aMatrix.at(i).at(j))
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
bool Matrix<T>::operator!=(const Matrix<T> &org)
{
    return !(*this == org);
}