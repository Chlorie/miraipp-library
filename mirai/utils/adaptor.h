#pragma once

#include <utility>

// Provides adaptors for reducing boilerplates
namespace mirai::adp
{
    /**
     * \brief Provides implementation of equality operators based on user defined operator==
     * \tparam T The first type
     * \tparam U The second type
     * \remarks You should implement T == U, and this adaptor will provide you with
     * T != U, U == T and U != T
     */
    template <typename T, typename U = T>
    struct Equal
    {
        friend bool operator!=(const T& lhs, const U& rhs) { return !(lhs == rhs); }
        friend bool operator==(const U& lhs, const T& rhs) { return rhs == lhs; }
        friend bool operator!=(const U& lhs, const T& rhs) { return !(rhs == lhs); }
    };

    template <typename T>
    struct Equal<T, T>
    {
        friend bool operator!=(const T& lhs, const T& rhs) { return !(lhs == rhs); }
    };

    /**
     * \brief Provides implementation of comparison operators based on
     * user defined operator== and operator<
     * \tparam T The first type
     * \tparam U The second type
     * \remarks If T and U are the same, T == T and T < T should be provided, otherwise,
     * T == U, T < U and U < T should be provided
     */
    template <typename T, typename U = T>
    struct Compare : Equal<T, U>
    {
        friend bool operator>(const T& lhs, const U& rhs) { return rhs < lhs; }
        friend bool operator<=(const T& lhs, const U& rhs) { return !(rhs < lhs); }
        friend bool operator>=(const T& lhs, const U& rhs) { return !(lhs < rhs); }
        friend bool operator>(const U& lhs, const T& rhs) { return rhs < lhs; }
        friend bool operator<=(const U& lhs, const T& rhs) { return !(rhs < lhs); }
        friend bool operator>=(const U& lhs, const T& rhs) { return !(lhs < rhs); }
    };

    template <typename T>
    struct Compare<T, T> : Equal<T, T>
    {
        friend bool operator>(const T& lhs, const T& rhs) { return rhs < lhs; }
        friend bool operator<=(const T& lhs, const T& rhs) { return !(rhs < lhs); }
        friend bool operator>=(const T& lhs, const T& rhs) { return !(lhs < rhs); }
    };

    /**
     * \brief Provides implementation of operator+ based on user defined operator+=
     * \tparam T The first type
     * \tparam U The second type
     */
    template <typename T, typename U>
    struct Concatenate
    {
        friend T operator+(T lhs, const U& rhs) { return lhs += rhs; }
        friend T operator+(T lhs, U&& rhs) { return lhs += std::move(rhs); }
    };
}
