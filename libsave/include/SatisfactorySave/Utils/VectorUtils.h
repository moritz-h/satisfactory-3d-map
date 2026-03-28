#pragma once

#include <span>
#include <vector>

namespace SatisfactorySave {
    template<typename T>
    inline std::size_t vector_bin_size(const std::vector<T>& vec) {
        return vec.size() * sizeof(T);
    }

    template<typename T>
    [[nodiscard]] std::span<const std::byte> as_bytes(const std::vector<T>& vec) noexcept {
        return std::as_bytes(std::span<const T>{vec});
    }
} // namespace SatisfactorySave
