#pragma once

#include <vector>

namespace mirai::utils
{
    /**
     * \brief Construct a vector using an "initializer list" but enables move semantics
     * \tparam T Type of element in the vector
     * \tparam Ts Initializer list type
     * \param list Initializer list
     * \return The constructed vector
     */
    template <typename T, typename... Ts>
    std::vector<T> make_vector(Ts&&... list)
    {
        std::vector<T> vec;
        vec.reserve(sizeof...(list));
        (vec.emplace_back(std::forward<Ts>(list)), ...);
        return vec;
    }
}
