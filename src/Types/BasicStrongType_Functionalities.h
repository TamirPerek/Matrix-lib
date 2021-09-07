#pragma once

#include "BasicStrongType.h"

#include <functional>
#include <iostream>
#include <memory>

namespace Functionalities
{
    template <typename T, template <typename> class crtpType>
    struct crtp
    {
        T &underlying()
        {
            return static_cast<T &>(*this);
        }
        T const &underlying() const
        {
            return static_cast<T const &>(*this);
        }
    };

    template <typename T>
    struct PreIncrementable : crtp<T, PreIncrementable>
    {
        T &operator++()
        {
            ++this->underlying().get();
            return this->underlying();
        }
    };

    template <typename T>
    struct PostIncrementable : crtp<T, PostIncrementable>
    {
        T &operator++(int)
        {
            this->underlying().get()++;
            return this->underlying();
        }
    };

    template <typename T>
    struct PreDecrementable : crtp<T, PreDecrementable>
    {
        T &operator--()
        {
            --this->underlying().get();
            return this->underlying();
        }
    };

    template <typename T>
    struct PostDecrementable : crtp<T, PostDecrementable>
    {
        T &operator--(int)
        {
            this->underlying().get()--;
            return this->underlying();
        }
    };

    template <typename T>
    struct BinaryAddable : crtp<T, BinaryAddable>
    {
        T operator+(T const &other) const { return T(this->underlying().get() + other.get()); }
        T &operator+=(T const &other)
        {
            this->underlying().get() += other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct UnaryAddable : crtp<T, UnaryAddable>
    {
        T operator+() const { return T(+this->underlying().get()); }
    };

    template <typename T>
    struct Addable : BinaryAddable<T>, UnaryAddable<T>
    {
        using BinaryAddable<T>::operator+;
        using UnaryAddable<T>::operator+;
    };

    template <typename T>
    struct BinarySubtractable : crtp<T, BinarySubtractable>
    {
        T operator-(T const &other) const { return T(this->underlying().get() - other.get()); }
        T &operator-=(T const &other)
        {
            this->underlying().get() -= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct UnarySubtractable : crtp<T, UnarySubtractable>
    {
        T operator-() const { return T(-this->underlying().get()); }
    };

    template <typename T>
    struct Subtractable : BinarySubtractable<T>, UnarySubtractable<T>
    {
        using UnarySubtractable<T>::operator-;
        using BinarySubtractable<T>::operator-;
    };

    template <typename T>
    struct Multiplicable : crtp<T, Multiplicable>
    {
        T operator*(T const &other) const
        {
            return T(this->underlying().get() * other.get());
        }
        T &operator*=(T const &other)
        {
            this->underlying().get() *= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct Divisible : crtp<T, Divisible>
    {
        T operator/(T const &other) const
        {
            return T(this->underlying().get() / other.get());
        }
        T &operator/=(T const &other)
        {
            this->underlying().get() /= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct Modulable : crtp<T, Modulable>
    {
        T operator%(T const &other) const
        {
            return T(this->underlying().get() % other.get());
        }
        T &operator%=(T const &other)
        {
            this->underlying().get() %= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct BitWiseInvertable : crtp<T, BitWiseInvertable>
    {
        T operator~() const
        {
            return T(~this->underlying().get());
        }
    };

    template <typename T>
    struct BitWiseAndable : crtp<T, BitWiseAndable>
    {
        T operator&(T const &other) const
        {
            return T(this->underlying().get() & other.get());
        }
        T &operator&=(T const &other)
        {
            this->underlying().get() &= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct BitWiseOrable : crtp<T, BitWiseOrable>
    {
        T operator|(T const &other) const
        {
            return T(this->underlying().get() | other.get());
        }
        T &operator|=(T const &other)
        {
            this->underlying().get() |= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct BitWiseXorable : crtp<T, BitWiseXorable>
    {
        T operator^(T const &other) const
        {
            return T(this->underlying().get() ^ other.get());
        }
        T &operator^=(T const &other)
        {
            this->underlying().get() ^= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct BitWiseLeftShiftable : crtp<T, BitWiseLeftShiftable>
    {
        T operator<<(T const &other) const
        {
            return T(this->underlying().get() << other.get());
        }
        T &operator<<=(T const &other)
        {
            this->underlying().get() <<= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct BitWiseRightShiftable : crtp<T, BitWiseRightShiftable>
    {
        T operator>>(T const &other) const
        {
            return T(this->underlying().get() >> other.get());
        }
        T &operator>>=(T const &other)
        {
            this->underlying().get() >>= other.get();
            return this->underlying();
        }
    };

    template <typename T>
    struct Comparable : crtp<T, Comparable>
    {
        bool operator<(T const &other) const
        {
            return this->underlying().get() < other.get();
        }
        bool operator>(T const &other) const
        {
            return other.get() < this->underlying().get();
        }
        bool operator<=(T const &other) const
        {
            return !(other.get() < this->underlying().get());
        }
        bool operator>=(T const &other) const
        {
            return !(*this < other);
        }
        friend bool operator==(Comparable<T> const &self, T const &other)
        {
            return !(self < other) && !(other.get() < self.underlying().get());
        }
        bool operator!=(T const &other) const
        {
            return !(*this == other);
        }
    };

    template <typename Destination>
    struct ImplicitlyConvertibleTo
    {
        template <typename T>
        struct templ : crtp<T, templ>
        {
            operator Destination() const
            {
                return this->underlying().get();
            }
        };
    };

    template <typename BasicStrongType_>
    struct FunctionCallable;

    template <typename T, template <typename> class... Skills>
    struct FunctionCallable<BasicStrongType<T, Skills...>> : crtp<BasicStrongType<T, Skills...>, FunctionCallable>
    {
        operator T const &() const
        {
            return this->underlying().get();
        }
        operator T &()
        {
            return this->underlying().get();
        }
    };

    template <typename BasicStrongType_>
    struct MethodCallable;

    template <typename T, template <typename> class... Skills>
    struct MethodCallable<BasicStrongType<T, Skills...>> : crtp<BasicStrongType<T, Skills...>, MethodCallable>
    {
        std::remove_reference_t<T> const *operator->() const
        {
            return std::addressof(this->underlying().get());
        }
        std::remove_reference_t<T> *operator->()
        {
            return std::addressof(this->underlying().get());
        }
    };

    template <typename BasicStrongType_>
    struct Callable
        : FunctionCallable<BasicStrongType_>,
          MethodCallable<BasicStrongType_>
    {
    };

    template <typename T>
    struct Arithmetic
        : PreIncrementable<T>,
          PostIncrementable<T>,
          PreDecrementable<T>,
          PostDecrementable<T>,
          Addable<T>,
          Subtractable<T>,
          Multiplicable<T>,
          Divisible<T>,
          Modulable<T>,
          BitWiseInvertable<T>,
          BitWiseAndable<T>,
          BitWiseOrable<T>,
          BitWiseXorable<T>,
          BitWiseLeftShiftable<T>,
          BitWiseRightShiftable<T>,
          Comparable<T>
    {
    };
} // namespace Functionalities