#pragma once

#include <string>
#include <cstddef>

#include "BasicStrongType.h"
#include "BasicStrongType_Functionalities.h"

class Row
    : public BasicStrongType<std::size_t, Functionalities::Addable, Functionalities::Subtractable, Functionalities::PostIncrementable, Functionalities::PostDecrementable, Functionalities::Comparable>
{
public:
    using BasicStrongType<std::size_t, Functionalities::Addable, Functionalities::Subtractable, Functionalities::PostIncrementable, Functionalities::PostDecrementable, Functionalities::Comparable>::BasicStrongType;

    constexpr Row() = default;
    ~Row() = default;

    constexpr explicit Row(const Row &_in) noexcept
        : BasicStrongType{_in.data}
    {
    }
    constexpr explicit Row(Row &&_in) noexcept
        : BasicStrongType{std::move(_in.data)}
    {
    }

    constexpr Row &operator=(const std::size_t &_in)
    {
        data = _in;
        return *this;
    }
    constexpr Row &operator=(std::size_t &&_in)
    {
        data = std::move(_in);
        return *this;
    }
    constexpr Row &operator=(const Row &_in) noexcept
    {
        data = _in.data;
        return *this;
    }
    constexpr Row &operator=(Row &&_in) noexcept
    {
        data = std::move(_in.data);
        return *this;
    }

    // constexpr operator std::size_t() const noexcept { return data; }
    // constexpr operator bool() const noexcept { return (data > 0); }

    const std::string to_string() const noexcept { return std::to_string(data); }
    const std::wstring to_wstring() const noexcept { return std::to_wstring(data); }

    constexpr bool operator==(const std::size_t &_in) { return (data == _in); }
    constexpr bool operator!=(const std::size_t &_in) { return (data != _in); }
    constexpr bool operator<=(const std::size_t &_in) { return (data <= _in); }
    constexpr bool operator<(const std::size_t &_in) { return (data < _in); }
    constexpr bool operator>=(const std::size_t &_in) { return (data >= _in); }
    constexpr bool operator>(const std::size_t &_in) { return (data > _in); }
};

constexpr bool operator==(const std::size_t &_in, const Row &_self) { return (_in == _self.get()); }
constexpr bool operator!=(const std::size_t &_in, const Row &_self) { return (_in != _self.get()); }
constexpr bool operator<=(const std::size_t &_in, const Row &_self) { return (_in <= _self.get()); }
constexpr bool operator<(const std::size_t &_in, const Row &_self) { return (_in < _self.get()); }
constexpr bool operator>=(const std::size_t &_in, const Row &_self) { return (_in >= _self.get()); }
constexpr bool operator>(const std::size_t &_in, const Row &_self) { return (_in > _self.get()); }

namespace std
{
    template <>
    struct less<Row>
    {
        bool operator()(const Row &lhs, const Row &rhs) const
        {
            return lhs.get() < rhs.get();
        }
    };
}