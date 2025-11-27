#pragma once
#include <string_view>

/// @brief A transparent hasher for strings to allow heterogeneous lookup in unordered containers.
struct TransparentStringHash
{
    using is_transparent = void;

    size_t operator()(std::string_view sv) const noexcept
    {
        return std::hash<std::string_view>{}(sv);
    }
};