#pragma once

#include <string>
#include <cstddef>

#include "BasicStrongType.h"
#include "BasicStrongType_Functionalities.h"

class Column
    : public BasicStrongType<std::size_t, Functionalities::Addable, Functionalities::Subtractable, Functionalities::PostIncrementable, Functionalities::PostDecrementable, Functionalities::Comparable>
{
public:
    using BasicStrongType<std::size_t, Functionalities::Addable, Functionalities::Subtractable, Functionalities::PostIncrementable, Functionalities::PostDecrementable, Functionalities::Comparable>::BasicStrongType;

    constexpr Column() = default;
    ~Column() = default;

    constexpr explicit Column(const Column &_in) noexcept
        : BasicStrongType{_in.data}
    {
    }
    constexpr explicit Column(Column &&_in) noexcept
        : BasicStrongType{std::move(_in.data)}
    {
    }

    constexpr Column &operator=(const std::size_t &_in)
    {
        data = _in;
        return *this;
    }
    constexpr Column &operator=(std::size_t &&_in)
    {
        data = std::move(_in);
        return *this;
    }
    constexpr Column &operator=(const Column &_in) noexcept
    {
        data = _in.data;
        return *this;
    }
    constexpr Column &operator=(Column &&_in) noexcept
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

constexpr bool operator==(const std::size_t &_in, const Column &_self) { return (_in == _self.get()); }
constexpr bool operator!=(const std::size_t &_in, const Column &_self) { return (_in != _self.get()); }
constexpr bool operator<=(const std::size_t &_in, const Column &_self) { return (_in <= _self.get()); }
constexpr bool operator<(const std::size_t &_in, const Column &_self) { return (_in < _self.get()); }
constexpr bool operator>=(const std::size_t &_in, const Column &_self) { return (_in >= _self.get()); }
constexpr bool operator>(const std::size_t &_in, const Column &_self) { return (_in > _self.get()); }

namespace std
{
    template <>
    struct less<Column>
    {
        bool operator()(const Column &lhs, const Column &rhs) const
        {
            return lhs.get() < rhs.get();
        }
    };
}