#pragma once

#include <span>
#include <vector>

namespace SatisfactorySave {
    template<typename T>
    inline std::size_t vector_bin_size(const std::vector<T>& vec) {
        return vec.size() * sizeof(T);
    }

    template<typename T>
    std::span<const char> vector_to_char_span(const std::vector<T>& vec) {
        return std::span<const char>(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(T));
    }
} // namespace SatisfactorySave
