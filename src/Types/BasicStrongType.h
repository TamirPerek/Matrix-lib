#pragma once

// Enable empty base class optimization with multiple inheritance on Visual Studio.
#if defined(_MSC_VER) && _MSC_VER >= 1910
#define FLUENT_EBCO __declspec(empty_bases)
#else
#define FLUENT_EBCO
#endif
template <typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;

template <typename T, template <typename> class... Skills>
class FLUENT_EBCO BasicStrongType : public Skills<BasicStrongType<T, Skills...>>...
{
protected:
    T data{};

public:
    using UnderlyingType = T;

    template <typename T_ = T, typename = std::enable_if<std::is_default_constructible<T>::value, void>>
    constexpr BasicStrongType() noexcept(std::is_nothrow_constructible<T>::value)
        : data{}
    {
    }

    explicit constexpr BasicStrongType(T const &value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        : data{value}
    {
    }

    template <typename T_ = T, typename = IsNotReference<T_>>
    explicit constexpr BasicStrongType(T &&value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : data(std::move(value))
    {
    }

    // get
    constexpr T &get() noexcept
    {
        return data;
    }
    constexpr operator const T&() noexcept
    {
        return data;
    }

    constexpr std::remove_reference_t<T> const &get() const noexcept
    {
        return data;
    }

    // conversions
    using ref = BasicStrongType<T &, Skills...>;
    operator ref()
    {
        return ref(data);
    }

    struct argument
    {
        BasicStrongType operator=(T &&value) const
        {
            return BasicStrongType(std::forward<T>(value));
        }
        template <typename U>
        BasicStrongType operator=(U &&value) const
        {
            return BasicStrongType(std::forward<U>(value));
        }

        argument() = default;
        argument(argument const &) = delete;
        argument(argument &&) = delete;
        argument &operator=(argument const &) = delete;
        argument &operator=(argument &&) = delete;
    };
};