#include "Types/row.h"

#include <vector>
#include <cmath>
#include <type_traits>
#include <numeric>
#include <algorithm>

template <typename T>
class Vector
{
private:
    Row m_Rows{};
    std::vector<T> m_Data{};

    Vector() noexcept = default;
    explicit Vector(const Row &xRows) noexcept;
    explicit Vector(const std::vector<T> &xVector) noexcept;

public:
    static Vector<T> create() noexcept;
    static Vector<T> create(const Row &xRows) noexcept;
    static Vector<T> create(const std::vector<T> &xVector) noexcept;
    static Vector<T> create(const Vector<T> &xVector) noexcept;

    const std::vector<T> &getData() const noexcept;
    std::vector<T> &getData() noexcept;
    constexpr const Row &getRows() const noexcept;

    void erase() noexcept;

    bool operator==(const Vector<T> &xOther) noexcept;
    bool operator!=(const Vector<T> &xOther) noexcept;
    const T &operator[](const std::size_t &xPos) const noexcept(false) { return m_Data[xPos]; }
    T &operator[](const std::size_t &xPos) noexcept(false) { return m_Data[xPos]; }
    const T &at(const std::size_t &xPos) const noexcept(false) { return m_Data.at(xPos); }
    T &at(const std::size_t &xPos) noexcept(false) { return m_Data.at(xPos); }
    std::size_t size() const noexcept { return m_Data.size(); }

    // Vector math
    constexpr T magnitude() const noexcept;
    constexpr void normalize() noexcept;
    bool opposite(const Vector<T> &xOther) const noexcept;
    bool parallel(const Vector<T> &xOther) const noexcept;
    bool antiParallel(const Vector<T> &xOther) const noexcept;
    T dotProduct(const Vector<T> &) const noexcept;
    Vector<T> crossProduct(const Vector<T> &) const noexcept;
    Vector<T> &operator+=(const Vector<T> &) noexcept;
    Vector<T> operator+(const Vector<T> &) const noexcept;
    Vector<T> &operator+=(const T &) noexcept;
    Vector<T> operator+(const T &) const noexcept;
    Vector<T> &operator-=(const Vector<T> &) noexcept;
    Vector<T> operator-(const Vector<T> &) const noexcept;
    Vector<T> &operator-=(const T &) noexcept;
    Vector<T> operator-(const T &) const noexcept;
    Vector<T> &operator*=(const T &) noexcept;
    Vector<T> operator*(const T &) const noexcept;

    // Iterator
    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        explicit Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator &operator++()
        {
            m_ptr++;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };

    class Const_Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        explicit Const_Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Const_Iterator &operator++()
        {
            m_ptr++;
            return *this;
        }
        Const_Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Const_Iterator &a, const Const_Iterator &b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Const_Iterator &a, const Const_Iterator &b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&(*m_Data.begin())); }
    Iterator end() { return Iterator(&(*m_Data.end())); }

    Const_Iterator begin() const { return Const_Iterator(&(*m_Data.begin())); }
    Const_Iterator end() const { return Const_Iterator(&(*m_Data.end())); }
};

template <typename T>
Vector<T>::Vector(const Row &xRows) noexcept
{
    if (xRows.get() > 0UL)
    {
        m_Data.resize(xRows.get());
        m_Rows = xRows;
    }
}
template <typename T>
Vector<T>::Vector(const std::vector<T> &xVector) noexcept
    : m_Data{xVector}, m_Rows{xVector.size()}
{
}
template <typename T>
Vector<T> Vector<T>::create() noexcept
{
    return Vector<T>{};
}
template <typename T>
Vector<T> Vector<T>::create(const Row &xRows) noexcept
{
    return Vector<T>{xRows};
}
template <typename T>
Vector<T> Vector<T>::create(const std::vector<T> &xVector) noexcept
{
    return Vector<T>{xVector};
}
template <typename T>
Vector<T> Vector<T>::create(const Vector<T> &xVector) noexcept
{
    return Vector<T>{xVector};
}
template <typename T>
const std::vector<T> &Vector<T>::getData() const noexcept
{
    return m_Data;
}
template <typename T>
std::vector<T> &Vector<T>::getData() noexcept
{
    return m_Data;
}
template <typename T>
constexpr const Row &Vector<T>::getRows() const noexcept
{
    return m_Rows;
}
template <typename T>
void Vector<T>::erase() noexcept
{
    m_Rows = 0UL;
    m_Data.clear();
}

template <typename T>
bool Vector<T>::operator==(const Vector<T> &xOther) noexcept
{
    if (m_Rows != xOther.m_Rows)
        return false;

    for (std::size_t i = 0; i < m_Rows.get(); i++)
    {
        if (m_Data.at(i) != xOther.m_Data.at(i))
            return false;
    }
    return true;
}
template <typename T>
bool Vector<T>::operator!=(const Vector<T> &xOther) noexcept
{
    return !(*this == xOther);
}

template <typename T>
constexpr T Vector<T>::magnitude() const noexcept
{
    auto tSum = std::accumulate(m_Data.cbegin(), m_Data.cend(), static_cast<T>(0.0), [](T xSum, T xElem)
                                { return xSum + (xElem * xElem); });

#if defined(__clang__) || defined(_MSC_VER) // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79700 is not yet solved
    if constexpr (std::is_integral_v<T> || std::is_same_v<double, T>)
        return static_cast<T>(std::sqrt(tSum));
    else if constexpr (std::is_same_v<long double, T>)
        return static_cast<T>(std::sqrtl(tSum));
    else if constexpr (std::is_same_v<float, T>)
        return static_cast<T>(std::sqrtf(tSum));
    else
        return static_cast<T>(std::sqrt(tSum));
#else
    return static_cast<T>(std::sqrt(tSum));
#endif
}

template <typename T>
constexpr void Vector<T>::normalize() noexcept
{
    const auto tMagnitude = magnitude();

    std::transform(m_Data.cbegin(), m_Data.cend(), m_Data.begin(), [tMagnitude](T xElem)
                   { return xElem / tMagnitude; });
}

template <typename T>
bool Vector<T>::opposite(const Vector<T> &xOther) const noexcept
{
    if (xOther.size() != m_Data.size())
        return false;

    return std::equal(m_Data.cbegin(), m_Data.cend(), xOther.m_Data.cbegin(), xOther.m_Data.cend(), [](T xElem1, T xElem2)
                      {
                            const auto tToControl = xElem1 * static_cast<T>(-1);
                            return tToControl == xElem2; 
                      });
}

template <typename T>
bool Vector<T>::parallel(const Vector<T> &xOther) const noexcept
{
    if (xOther.size() != size())
        return false;

    const auto tToControl = m_Data.front() / xOther.m_Data.front();

    return std::equal(m_Data.cbegin(), m_Data.cend(), xOther.m_Data.cbegin(), xOther.m_Data.cend(), [&tToControl](auto xElem1, auto xElem2)
                      {
                            const auto tResult = xElem1 / xElem2;
                            if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
                                return (tResult - tToControl) <= std::numeric_limits<T>::epsilon();
                            else
                                return (tResult - tToControl) <= T::epsilon(); 
                      });
}
template <typename T>
bool Vector<T>::antiParallel(const Vector<T> &xOther) const noexcept
{
    if (xOther.size() != size())
        return false;

    const auto tToControl = (m_Data.front() / xOther.m_Data.front()) * static_cast<T>(-1);

    return std::equal(m_Data.cbegin(), m_Data.cend(), xOther.m_Data.cbegin(), xOther.m_Data.cend(), [&tToControl](auto xElem1, auto xElem2)
                      {
                            const auto tResult = (xElem1 / xElem2) * static_cast<T>(-1);
                            if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
                                return (tResult - tToControl) <= std::numeric_limits<T>::epsilon();
                            else
                                return (tResult - tToControl) <= T::epsilon(); 
                      });
}

template <typename T>
T Vector<T>::dotProduct(const Vector<T> &xOther) const noexcept
{
    T tResult{static_cast<T>(0)};

    if (xOther.size() != size())
        return tResult;

    for (std::size_t i = 0; i < size(); i++)
    {
        tResult += xOther.at(i) * this->at(i);
    }

    return tResult;
}

template <typename T>
Vector<T> Vector<T>::crossProduct(const Vector<T> &xOther) const noexcept
{
    if (size() != xOther.size() || size() != 3)
        return create();

    std::vector<T> tTemp;
    tTemp.emplace_back(at(1) * xOther.at(2) - at(2) * xOther.at(1));
    tTemp.emplace_back(at(2) * xOther.at(0) - at(0) * xOther.at(2));
    tTemp.emplace_back(at(0) * xOther.at(1) - at(1) * xOther.at(0));

    return create(tTemp);
}

template <typename T>
Vector<T> &Vector<T>::operator+=(const Vector<T> &xOther) noexcept
{
    auto tResult = *this + xOther;
    *this = std::move(tResult);
    return *this;
}
template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T> &xOther) const noexcept
{
    Vector<T> tResult{Row{size()}};

    if (xOther.size() != size())
        return tResult;

    for (std::size_t i = 0; i < size(); ++i)
    {
        tResult.at(i) = this->at(i) + xOther.at(i);
    }

    return tResult;
}
template <typename T>
Vector<T> &Vector<T>::operator+=(const T &xOther) noexcept
{
    auto tResult = *this + xOther;
    *this = std::move(tResult);
    return *this;
}
template <typename T>
Vector<T> Vector<T>::operator+(const T &xOther) const noexcept
{
    Vector<T> tResult{Row{size()}};

    for (std::size_t i = 0; i < size(); ++i)
    {
        tResult.at(i) = this->at(i) + xOther;
    }

    return tResult;
}
template <typename T>
Vector<T> &Vector<T>::operator-=(const Vector<T> &xOther) noexcept
{
    auto tResult = *this - xOther;
    *this = std::move(tResult);
    return *this;
}
template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T> &xOther) const noexcept
{
    if (xOther.size() != size())
        return *this;

    Vector<T> tResult{Row{size()}};

    for (std::size_t i = 0; i < size(); ++i)
    {
        tResult.at(i) = this->at(i) - xOther.at(i);
    }

    return tResult;
}
template <typename T>
Vector<T> &Vector<T>::operator-=(const T &xOther) noexcept
{
    auto tResult = *this - xOther;
    *this = std::move(tResult);
    return *this;
}
template <typename T>
Vector<T> Vector<T>::operator-(const T &xOther) const noexcept
{
    Vector<T> tResult{Row{size()}};

    for (std::size_t i = 0; i < size(); ++i)
    {
        tResult.at(i) = this->at(i) - xOther;
    }

    return tResult;
}
template <typename T>
Vector<T> &Vector<T>::operator*=(const T &xOther) noexcept
{
    auto tResult = *this * xOther;
    *this = std::move(tResult);
    return *this;
}
template <typename T>
Vector<T> Vector<T>::operator*(const T &xOther) const noexcept
{
    Vector<T> tResult{Row{size()}};

    for (std::size_t i = 0; i < size(); ++i)
    {
        tResult.at(i) = this->at(i) * xOther;
    }

    return tResult;
}