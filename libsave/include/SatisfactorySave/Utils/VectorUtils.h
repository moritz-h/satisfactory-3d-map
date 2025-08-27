#pragma once

#include <vector>

namespace SatisfactorySave {
    template<typename T>
    inline std::size_t vector_bin_size(const std::vector<T>& vec) {
        return vec.size() * sizeof(T);
    }
} // namespace SatisfactorySave
